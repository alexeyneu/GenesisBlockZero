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

const uint32_t startNonce =0; /* 2083236893 */
const uint32_t unixtime =0; /* 1231006505 */
const uint64_t COIN = 100000000;
const uint64_t CENT = 1000000;
const uint32_t OP_CHECKSIG = 172; // This is expressed as 0xAC
const uint8_t pubscriptlength = 67; // 2 + 65 ,  if unknown at compile time ... 
//here are PODs coz there is no constructors and others
struct Transaction
{
	#pragma pack(1)
	uint8_t merkleHash[32];
	uint8_t *serializedData;
/* +0 */uint32_t version;   
	uint8_t numInputs; 
	uint8_t prevOutput[32];
	uint32_t prevoutIndex; // +41
	uint8_t *scriptSig; 
/* +0 */uint32_t sequence;
	uint8_t numOutputs; 
	uint64_t outValue; // +13
/* +0 */uint8_t  pubkeyScript[pubscriptlength];
	uint32_t locktime;   // +71
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

const	std::string bin2hex(const unsigned char *p, size_t len)
{
	std::stringstream f;
	f<<std::hex << std::setfill('0');
	for (int i = 0; i < len; i++)
		f << std::setw(2) << (int)p[i];
	return f.str();
}
size_t hex2bin(unsigned char *p , const char *hexstr,const size_t len)
{
	size_t wlen = 0;
	while ((wlen<len) && *hexstr && *(hexstr+1))    //last condition cause np if check fails on middle one.thats coz of short-circuit evaluation
        {
		sscanf(hexstr, "%2hhx",p++);   // 0 or 1 (maybe smth else too) . Slow stuff , np  		
		hexstr = hexstr+2;
		wlen++;
	}
	return  (wlen == len - 1)*len;     // zero if error .had enough 
}

int main(int argc, char *argv[])
{
	unsigned char hash1[32];
	char timestamp[255], pubkey[131];
	uint32_t timestamp_len = 0, scriptSig_len = 0, pubkey_len = 0;
	uint32_t nBits = 0;
	if((argc-1) < 3)
	{
		fprintf(stderr, "Usage: blockzero [options] <pubkey> \"<timestamp>\" <nBits>\n");
		return 0;		
	}
	pubkey_len = strlen(argv[1]) / 2; // One byte is represented as two hex characters, thus we divide by two to get real length.
	timestamp_len = strlen(argv[2]);
	if((pubkey_len != 65) || (strlen(argv[1])  % 2))
	{
		fprintf(stderr, "Invalid public key length! %s\n", argv[1]);
		return 0;
	}
	if(timestamp_len > 254 || timestamp_len < 1)
	{
		fprintf(stderr, "Size of timestamp is 0 or exceeds maximum length of 254 characters!\n");
		return 0;
	}	
	Transaction transaction={ {},0 ,/* drift */1 ,1 , {},0xFFFFFFFF ,0 ,0xFFFFFFFF ,1 ,50*COIN ,{} ,0 };
	strncpy(pubkey, argv[1], sizeof(pubkey));		
	strncpy(timestamp, argv[2], sizeof(timestamp));
	sscanf(argv[3], "%d", (long unsigned int *)&nBits); 
	// pubkey to bytes , then prepend pubkey size, then append the OP_CHECKSIG byte	
	transaction.pubkeyScript[0] = 0x41; // size. A public key is 32 bytes X coordinate, 32 bytes Y coordinate and one byte 0x04, so 65 bytes i.e 0x41 in Hex.
	hex2bin(transaction.pubkeyScript + 1, pubkey, 65); // No error checking, yeah.
	transaction.pubkeyScript[66] = OP_CHECKSIG;
	/*public key is done*/
	transaction.scriptSig =(uint8_t *) malloc(8); //max size of first part 1 + 4max + 1 + 1 + 1
	uint32_t scriptSig_pos = 0;
	short pl;
	/* nbits related stuff , first is real size of value in bytes , next is value with 
	   null bytes on the left removed  */
	// statement (smth > 0) returns 0 or 1.  !! turns value in brackets to !=0 condition check. say !!5 is 1
	transaction.scriptSig[scriptSig_pos++] = pl = 0x01+!!(nBits >> 8)+!!(nBits >> 16)+!!(nBits >> 24);  
	memcpy(transaction.scriptSig + scriptSig_pos, &nBits, pl);
	scriptSig_pos = scriptSig_pos + pl;
	// nobody knows what that means
	transaction.scriptSig[scriptSig_pos++] = 0x01;
	transaction.scriptSig[scriptSig_pos++] = 0x04;
	transaction.scriptSig[scriptSig_pos++] = timestamp_len;  
	scriptSig_len = scriptSig_pos + timestamp_len;
	transaction.scriptSig = (uint8_t*)realloc(transaction.scriptSig, scriptSig_len);
	memcpy(transaction.scriptSig+scriptSig_pos, (const unsigned char *)timestamp, timestamp_len);
	/*scriptsig is done*/
	uint32_t serializedLen = 
	4    // tx version  
	+1   // number of inputs
	+32  // hash of previous output
	+4   // previous output's index
	+1   // 1 byte for the size of scriptSig (?)
	+scriptSig_len
	+4   // size of sequence
	+1   // number of outputs
	+8   // 8 bytes for coin value
	+1   // 1 byte to represent size of the pubkey Script
	+pubscriptlength
	+4;   // 4 bytes for lock time

	uint32_t serializedData_pos = 0;
	transaction.serializedData = (uint8_t *)malloc(serializedLen);
	memcpy(transaction.serializedData, &transaction.version, 41);  
	/* fo' sho' */
//	std::cout << offsetof(Transaction,prevoutIndex) + sizeof(uint32_t) - offsetof(Transaction,version);	
	serializedData_pos += 41; 
	transaction.serializedData[serializedData_pos++] = scriptSig_len;
	memcpy(transaction.serializedData+serializedData_pos, transaction.scriptSig, scriptSig_len);
	serializedData_pos += scriptSig_len;
	memcpy(transaction.serializedData+serializedData_pos, &transaction.sequence, 13);
	serializedData_pos += 13;
	transaction.serializedData[serializedData_pos++] = pubscriptlength;
	memcpy(transaction.serializedData+serializedData_pos, &transaction.pubkeyScript, 71); 
	// hash it with SHA256 and then hash that result to get merkle hash
	SHA256(transaction.serializedData, serializedLen, hash1);
	SHA256(hash1, 32, transaction.merkleHash);
	std::string merkleHash = bin2hex(transaction.merkleHash, 32);
	std::reverse(transaction.merkleHash,transaction.merkleHash +32); 
	std::string merkleHashSwapped = bin2hex(transaction.merkleHash, 32);
	std::string txScriptSig = bin2hex(transaction.scriptSig, scriptSig_len);
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
	free(transaction.serializedData);
	free(transaction.scriptSig);
	return 0;
}
