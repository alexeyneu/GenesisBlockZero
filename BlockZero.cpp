#include <cstdint>
#include <ctime>
#include <openssl/sha.h>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <sstream>

#ifdef _MSC_VER
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")
#endif

const uint32_t startNonce =2083236893; /* 2083236893 */
const uint32_t unixtime =1231006505; /* 1231006505 */
const uint64_t COIN = 100000000;
const uint64_t CENT = 1000000;
const uint32_t OP_CHECKSIG = 172; // This is expressed as 0xAC
const uint8_t pubscriptlength = 67; // 2 + 65 ,  if unknown at compile time ... 
//here are PODs coz there is no constructors and others
struct Transaction
{
	#pragma pack(1)
	uint8_t merkleHash[32];
/* +0 */uint32_t version;   
	uint8_t numInputs; 
	uint8_t prevOutput[32];
	uint32_t prevoutIndex; // +41
/* +0 */uint32_t sequence;
	uint8_t numOutputs; 
	uint64_t outValue; 
	uint8_t pubscriptlength;
	uint8_t  pubkeyScript[::pubscriptlength];
	uint32_t locktime;   // +85
} ;
struct blockheader 
{
	#pragma pack(1)
	uint32_t blockversion;
	unsigned char hashPrevBlock[32];
	unsigned char merk[32];
	uint32_t unixtime;
	uint32_t nBits;
	uint32_t startNonce;
};
struct blockhash {
	#pragma pack(1)
	unsigned char bl[28];
	uint32_t checkbytes;
};

const	std::string bin2hex(const unsigned char *p, size_t length)
{
	std::stringstream f;
	f<<std::hex << std::setfill('0');
	for (int i = 0; i < length; i++)
		f << std::setw(2) << (int)p[i];
	return f.str();
}
size_t hex2bin(unsigned char *p , const char *hexstr,const size_t length)
{
	size_t wcount = 0;
	while ((wcount<length) && *hexstr && *(hexstr+1))    //last condition cause np if check fails on middle one.thats coz of short-circuit evaluation
        {
		sscanf(hexstr, "%2hhx",p++);   // 0 or 1 (maybe smth else too) . Slow stuff , np  		
		hexstr = hexstr+2;
		wcount++;
	}
	return  (wcount == length - 1)*length;     // zero if error .had enough 
}

int main(int argc, char *argv[])
{
	unsigned char hash1[32];
	char timestamp[255], pubkey[131];
	uint32_t timestamp_length = 0,  pubkey_length = 0, nBits = 0;
	if((argc-1) < 3)
	{
		fprintf(stderr, "Usage: blockzero [options] <pubkey> \"<timestamp>\" <nBits>\n");
		return 0;		
	}
	pubkey_length = strlen(argv[1]) / 2; // One byte is represented as two hex characters, thus we divide by two to get real length.
	timestamp_length = strlen(argv[2]);
	if((pubkey_length != 65) || (strlen(argv[1])  % 2))
	{
		fprintf(stderr, "Invalid public key length! %s\n", argv[1]);
		return 0;
	}
	if(timestamp_length > 254 || timestamp_length < 1)
	{
		fprintf(stderr, "Size of timestamp is 0 or exceeds maximum length of 254 characters!\n");
		return 0;
	}	
	Transaction transaction={ {},/* version */1 ,1 , {},0xFFFFFFFF ,/* sequ */0xFFFFFFFF ,1 ,50*COIN ,pubscriptlength,{} ,0 };
	strncpy(pubkey, argv[1], sizeof(pubkey));		
	strncpy(timestamp, argv[2], sizeof(timestamp));
	sscanf(argv[3], "%d", (long unsigned int *)&nBits); 
	// pubkey to bytes , then prepend pubkey size, then append the OP_CHECKSIG byte	
	transaction.pubkeyScript[0] = 0x41; // size. A public key is 32 bytes X coordinate, 32 bytes Y coordinate and one byte 0x04, so 65 bytes i.e 0x41 in Hex.
	hex2bin(transaction.pubkeyScript + 1, pubkey, 65); // No error checking, yeah.
	transaction.pubkeyScript[66] = OP_CHECKSIG;
	/*public key is done*/
	
	uint8_t *serializedData =(uint8_t *) malloc(41 + 1 + 7); //7 = max size of scriptSig first part , 4max + 1 + 1 + 1
	uint32_t serializedData_pos = 0;
	memcpy(serializedData, &transaction.version, 41);  
	serializedData_pos = serializedData_pos + 41;
	/* fo' sho' */ 
//	std::cout << offsetof(Transaction,prevoutIndex) + 4 /* size of prevoutIndex */ - offsetof(Transaction,version);/* OK output : " 41 " */	
	serializedData_pos++;  // +41 byte reserved 
	short pl;
/*	nbits related stuff , first is real size of value in bytes , next is value with null bytes on the left removed 
	statement (smth > 0) returns 0 or 1.  !! turns value in brackets to !=0 condition check. say !!5 is 1 */
	serializedData[serializedData_pos++] = pl = 0x01+!!(nBits >> 8)+!!(nBits >> 16)+!!(nBits >> 24);  // size of nbits , not neccesary same as sizeof(nbits)   
	memcpy(serializedData + serializedData_pos, &nBits, pl);
	serializedData_pos = serializedData_pos + pl;
	serializedData[serializedData_pos++] = 0x01;	    // nobody knows what this means
	serializedData[serializedData_pos++] = 0x04;	    // nobody knows what this means
	serializedData[serializedData_pos++] = timestamp_length;  
	uint32_t  scriptSig_length = serializedData_pos  - 42 + timestamp_length; // serializedData_pos inc'ed already to go as size
	serializedData[41] = scriptSig_length;
	
	uint32_t serializedLength = 
	4    // tx version  
	+1   // number of inputs
	+32  // hash of previous output
	+4   // previous output's index
	+1   // 1 byte for the size of scriptSig (?)
	+scriptSig_length
	+4   // size of sequence
	+1   // number of outputs
	+8   // 8 bytes for coin value
	+1   // 1 byte to represent size of the pubkey Script
	+pubscriptlength
	+4;   // 4 bytes for lock time	
	
	serializedData = (uint8_t*)realloc(serializedData ,serializedLength );
	memcpy(serializedData + serializedData_pos, (const unsigned char *)timestamp, timestamp_length);
	serializedData_pos = serializedData_pos + timestamp_length;
	/*scriptsig is done*/
	memcpy(serializedData+serializedData_pos, &transaction.sequence, 85);
	// hash it with SHA256 and then hash that result to get merkle hash
	SHA256(serializedData, serializedLength , hash1);
	SHA256(hash1, 32, transaction.merkleHash);
	std::string merkleHash = bin2hex(transaction.merkleHash, 32);
	std::reverse(transaction.merkleHash,transaction.merkleHash +32); 
	std::string merkleHashSwapped = bin2hex(transaction.merkleHash, 32);
	std::string txScriptSig = bin2hex(serializedData + 42, scriptSig_length);
	std::string pubScriptSig = bin2hex(transaction.pubkeyScript, pubscriptlength);
	std::cout<<"\nCoinbase: "<< txScriptSig <<"\nPubkeyScript: "<<pubScriptSig <<"\nMerkle Hash: "<<merkleHash<<"\nByteswapped: "<< merkleHashSwapped << std::endl;
		printf("Generating block...\n\n");
		unsigned char  block_hash1[32];
		blockheader block_header={/* drift */ 1 , {} , {} , unixtime == 0 ? time(NULL) : unixtime, nBits , startNonce };
 		blockhash block_hashf;
		unsigned char* block_headerp=(unsigned char*)&block_header;
		unsigned char* block_hashfp=(unsigned char*)&block_hashf;
		std::reverse(transaction.merkleHash,transaction.merkleHash +32); 
		memcpy(&block_header.merk, transaction.merkleHash, 32);   // address of merk itself,not address that merk contains. 
		unsigned int counter=0, start = time(NULL);
		while(1)
		{
			SHA256(block_headerp, 80, block_hash1);
			SHA256(block_hash1, 32, block_hashfp);
			if(block_hashf.checkbytes == 0) // { .. , 0x00, 0x00, 0x00, 0x00 } check the last 4 bytes.
			{
				std::reverse(block_hashfp,block_hashfp +32);
				std::string blockHash = bin2hex(block_hashfp, 32);
				std::cout<<"\nBlock found!\nHash: " << blockHash  <<"\nNonce: " << block_header.startNonce << "\nUnix time: "<< block_header.unixtime << std::endl;
				break;
			}
			if(time(NULL)-start > 0)
			{
				std::cout<<counter<<" Hashes/s, Nonce "<< block_header.startNonce<<'\r';
				counter = 0;
				start = time(NULL);
			}
			if(block_header.startNonce == 0x100000000 - 1)
			{
				block_header.unixtime++; //trick is that to change pre-start time to find a block(really it's smth else) faster then nonce wraps
			}
			block_header.startNonce++;		//see what happens on bounds
			counter++;
		}
	free(serializedData);
	return 0;
}
