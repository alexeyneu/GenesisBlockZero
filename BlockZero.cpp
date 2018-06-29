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
#pragma comment(lib,"libssl.lib")
#endif

const uint32_t startNonce = 0 ; /* 2083236893 */
const uint32_t unixtime =  0 ; /* 1231006505 */
const uint64_t COIN = 100000000;
const uint32_t OP_CHECKSIG = 172; // This is expressed as 0xAC
const uint8_t pubscriptlength = 67; // 2 + 65 ,  if unknown at compile time ... 
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
	uint8_t pubscriptlength; //it isn't in the bitcoin-api tx block 
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
const	std::string bin2hex(const unsigned char *p, size_t length)
{
	std::stringstream f;
	f<<std::hex << std::setfill('0');
	for (int i = 0; i < length; i++) f << std::setw(2) << (int)p[i];
	return f.str();
}
size_t hex2bin(unsigned char *p , const char *hexstr,const size_t length)
{
	size_t wcount = 0;
	while ((wcount++ < length) && *hexstr && *(hexstr+1))    //last condition cause np if check fails on middle one.thats coz of short-circuit evaluation
	{
		sscanf(hexstr, "%2hhx",p++);  //7x slower than tables but doesnt metter 
		hexstr = hexstr+2;
	}
	return  --wcount;     // error check here is a waste  
}

int main(int argc, char *argv[])
{
	unsigned char hash1[32],serializedData[857];
	char timestamp[255], pubkey[131];
	uint32_t timestamp_length = 0,  nBits = 0;
	if((argc-1) < 3) { std::cerr << "Usage: blockzero [options] <pubkey> \"<timestamp>\" <nBits>\n"; return 0; }
	timestamp_length = strlen(argv[2]);
	if(strlen(argv[1])!=130) { std::cerr << "Invalid public key length! " << argv[1]; return 0; }
	if(timestamp_length > 254 || timestamp_length < 1) 
	{ std::cerr <<  "Size of timestamp is 0 or exceeds maximum length of 254 characters!\n"; return 0; }	
	Transaction transaction={ {},/* version */1 ,1 , {},0xFFFFFFFF ,/* sequ */0xFFFFFFFF ,1 ,50*COIN ,pubscriptlength, {0x41}/* (?)opcode or size */ ,0 };
	strncpy(pubkey, argv[1], sizeof(pubkey));		
	strncpy(timestamp, argv[2], sizeof(timestamp));
	sscanf(argv[3], "%d", (long unsigned int *)&nBits); 
	// pubkey to bytes ,  then append the OP_CHECKSIG byte	
	hex2bin(transaction.pubkeyScript + 1, pubkey, 65); // No error checking, yeah.
	transaction.pubkeyScript[66] = OP_CHECKSIG;
	short sizeone = offsetof(Transaction,sequence)/*end of Transaction.prevoutIndex + 1 , not bad . adress same as size lookin for*/ - offsetof(Transaction,version);/* OK output :  41  */
	memcpy(serializedData, &transaction.version, sizeone);
	uint32_t serializedData_pos = sizeone + 1;// 42nd byte reserved
	short pl = 0x01+(nBits >> 8>0)+(nBits >> 16>0)+(nBits >> 24>0);  // size of nbits , not neccesary same as sizeof(nbits)   
	memcpy(serializedData+serializedData_pos, &nBits, serializedData[serializedData_pos++]=pl);
	serializedData_pos = serializedData_pos + pl;
	*(short*)(serializedData+serializedData_pos) = 0x0401;
	serializedData_pos=serializedData_pos+2;
	serializedData[serializedData_pos++] = timestamp_length;  
	uint32_t  scriptSig_length = serializedData_pos  - sizeone - 1 + timestamp_length; // serializedData_pos inc'ed already to go as size
	serializedData[sizeone] = scriptSig_length;
	short sizetwo = offsetof(Transaction,locktime)/* same stuff , +1 by address */ + 4/* last member size*/ - offsetof(Transaction,sequence);/* OK output :  85  */

	uint32_t serializedLength = sizeone + 1/* 1 byte for the size of scriptSig (?)*/+ scriptSig_length + sizetwo;
	memcpy(serializedData + serializedData_pos, (const unsigned char *)timestamp, timestamp_length);
	serializedData_pos = serializedData_pos + timestamp_length;
	/*scriptsig is done*/
	memcpy(serializedData+serializedData_pos, &transaction.sequence, sizetwo);
	// hash it with SHA256 and then hash that result to get merkle hash
	SHA256(serializedData, serializedLength , hash1);
	SHA256(hash1, 32, transaction.merkleHash);
	std::string merkleHash = bin2hex(transaction.merkleHash, 32);
	std::reverse(transaction.merkleHash,transaction.merkleHash +32); 
	std::string merkleHashSwapped = bin2hex(transaction.merkleHash, 32);
	std::string txScriptSig = bin2hex(serializedData + sizeone + 1, scriptSig_length);
	std::string pubScriptSig = bin2hex(transaction.pubkeyScript, pubscriptlength);
	std::cout << "\nCoinbase: "<< txScriptSig <<"\nPubkeyScript: "<< pubScriptSig << "\nMerkle Hash: "<< merkleHash <<"\nByteswapped: "<< merkleHashSwapped << std::endl << "Generating block...\n\n";
		unsigned char  block_hash1[32] , block_hashfp[32];
		blockheader block_header={1 , {} , {} , unixtime == 0 ? time(NULL) : unixtime, nBits , startNonce };
		std::reverse(transaction.merkleHash,transaction.merkleHash +32); 
		memcpy(&block_header.merk, transaction.merkleHash, 32);   // address of merk itself,not address that merk contains. 
		unsigned int counter=0, start = time(NULL);
		while(1)
		{
			SHA256((unsigned char*)&block_header, 80, block_hash1);
			SHA256(block_hash1, 32, block_hashfp);
			if(*(uint32_t*)(block_hashfp + 28) == 0) // { .. , 0x00, 0x00, 0x00, 0x00 } check the last 4 bytes.
			{
				std::reverse(block_hashfp,block_hashfp +32);
				std::cout << "\nBlock found!\nHash: " << bin2hex(block_hashfp, 32) <<"\nNonce: " << block_header.startNonce << "\nUnix time: "<< block_header.unixtime << std::endl;
				break;
			}
			if(time(NULL)-start > 0)
			{
				std::cout << counter<<" Hashes/s, Nonce "<< block_header.startNonce<<'\r';
				counter = 0;
				start = time(NULL);
			}
			if(block_header.startNonce++ == 0x100000000 - 1) block_header.unixtime++; //trick is that to change pre-start time to find a block(really it's smth else) faster then nonce wraps
			counter++;
		}
	return 0;
}
