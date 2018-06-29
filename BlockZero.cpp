#include <ctime>
#include <openssl/sha.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
const uint32_t startNonce = 0; // 2083236893; 
const uint32_t unixtime = 0; //1231006505;
const uint64_t COIN = 100000000;
const uint32_t OP_CHECKSIG = 172; // This is expressed as 0xAC
const uint8_t pubscriptlength = 67; // 2 + 65 ,  if unknown at compile time ... 
struct Transaction {
#pragma pack(1)
/* +0 */uint32_t version;   	/* vs bitcoin-api tx block : merkleHash[32] removed */
	uint8_t  numInputs; 
	uint8_t  prevOutput[32];
	uint32_t prevoutIndex; // +41
/* +0 */uint32_t sequence;
	uint8_t  numOutputs; 
	uint64_t outValue; 
	uint8_t  pubscriptlength; //it isn't in the bitcoin-api tx block also
	uint8_t  pubkeyScript[::pubscriptlength];
	uint32_t locktime;   // +85
} ;
struct blockheader {
#pragma pack(1)
	uint32_t blockversion;
	unsigned char hashPrevBlock[32];
	unsigned char merk[32];
	uint32_t unixtime;
	uint32_t nBits;
	uint32_t startNonce;
};
const	std::string bin2hex(const unsigned char *p, size_t length) {
	std::stringstream f;
	f<<std::hex << std::setfill('0');
	for (int i = 0; i < length; i++) f << std::setw(2) << (int)p[i];
	return f.str();
}
size_t hex2bin(unsigned char *p , const char *hexstr,const size_t length) {
	size_t wcount = 0;
	while ( wcount++ < length && *hexstr && *(hexstr + 1)) {    //last condition cause np if check fails on middle one.thats coz of short-circuit evaluation
		sscanf(hexstr, "%2hhx",p++);  //7x slower than tables but doesnt metter 
		hexstr = hexstr+2;
	}
	return  --wcount;     // error check here is a waste  
}
int main(int argc, char *argv[])
{
	unsigned char hash1[32], serializedData[857];
	uint32_t nBits = 0;
	if((argc-1) < 3) { std::cerr << "Usage: blockzero [options] <pubkey> \"<timestamp>\" <nBits>\n"; return -3; }
	if(strlen(argv[1])!=130) { std::cerr << "Invalid public key length! " << argv[1]; return -1; }
	if(strlen(argv[2]) > 254 || strlen(argv[2]) < 1) { std::cerr <<  "Size of timestamp is 0 or exceeds maximum length of 254 characters!\n"; return -2; }	
	if( sscanf(argv[3], "%d", (long unsigned int *)&nBits) || sscanf(argv[3], "%x", (long unsigned int *)&nBits)) ; 
	else { std::cerr << "where's nBits " << std::endl; return -1; } // short-circuit
	Transaction transaction = {/*version*/1,/*inputs*/1, {},/*poutindex*/0xFFFFFFFF,/*sequ*/0xFFFFFFFF,/*outputs*/1 ,50*COIN ,pubscriptlength, {0x41}/*pubscript[0]:(?)opcode or size */};
	std::string pubkey(argv[1]) , timestamp(argv[2]);		
	hex2bin(transaction.pubkeyScript + 1, pubkey.c_str(), pubkey.length()/2); 	// pubkey to bytes ,  then append the OP_CHECKSIG byte	
	transaction.pubkeyScript[pubscriptlength - 1] = OP_CHECKSIG;
	short sizeone = offsetof(Transaction, sequence);  /*, OK output :  41  */
	short sizetwo = sizeof transaction -  sizeone;/* OK output :  85  */
	memcpy(serializedData, &transaction, sizeone);
	uint32_t serializedData_pos = sizeone + 1;// 42nd byte reserved
	short pl = 0x01 + (nBits >> 8 > 0) + (nBits >> 16>0)+(nBits >> 24>0);  // size of nbits , not neccesary same as sizeof(nbits)   
	memcpy(serializedData + serializedData_pos, &nBits, serializedData[serializedData_pos++]/*a:1*/ = pl); // scriptSig {
	serializedData_pos = serializedData_pos + pl;
	*(short*)(serializedData + serializedData_pos) = 0x0401/*b:2*/; // {0x01,0x04} ,script op's 
	serializedData_pos = serializedData_pos + 2;
	memcpy(serializedData + serializedData_pos, timestamp.c_str(), serializedData[serializedData_pos++]/*c:1*/ = timestamp.length());  // scriptSig end
	memcpy(serializedData + serializedData_pos + timestamp.length(), &transaction.sequence, sizetwo);
	uint32_t scriptSig_length = serializedData[sizeone] = pl + 4/*abc round-up*/ + timestamp.length(); 
	SHA256(serializedData, sizeof transaction + 1 + scriptSig_length, hash1);  /* + 1 coz of serializedData[sizeone]*/
	blockheader block_header = {1/*version*/,{}/*hprev*/,{}/*merk*/,unixtime == 0 ? time(NULL) : unixtime ,nBits ,startNonce };
	SHA256(hash1, 32, block_header.merk);// hash it with SHA256 and then hash that result to get merkle hash
	std::string merkleHash = bin2hex( block_header.merk, 32);
	std::reverse(block_header.merk,block_header.merk +32); 
	std::string merkleHashSw = bin2hex(block_header.merk, 32); 
	std::reverse(block_header.merk,block_header.merk +32); 
	std::string txScriptSig = bin2hex(serializedData + sizeone + 1/*same*/, scriptSig_length);
	std::string pubScriptSig = bin2hex(transaction.pubkeyScript, pubscriptlength);
	std::cout <<"\nCoinbase: "<< txScriptSig <<"\nPubkeyScript: "<< pubScriptSig <<"\nMerkle Hash: "<< merkleHash <<"\nByteswapped: "<< merkleHashSw <<"\nGenerating block...\n\n";
		unsigned char  block_hash1[32] , block_hashfp[32];
		unsigned int counter=0, start = time(NULL);
	while(1) {
		SHA256((unsigned char*)&block_header, 80, block_hash1);
		SHA256(block_hash1, 32, block_hashfp);
		if(*(uint32_t *)(block_hashfp + 28) == 0) {// { .. , 0x00, 0x00, 0x00, 0x00 }
			std::reverse(block_hashfp,block_hashfp +32);
			std::cout << "\nBlock found!\nHash: " << bin2hex(block_hashfp, 32) <<"\nNonce: " << block_header.startNonce << "\nUnix time: "<< block_header.unixtime << std::endl;
			break;
		}
		if(time(NULL) - start > 0) {
			std::cout << counter <<" Hashes/s, Nonce "<< block_header.startNonce<<'\r';
			start = time((time_t *)(counter = 0));
		}
		if( ++block_header.startNonce == 0 ) block_header.unixtime++ ; //trick is that to change pre-start time to find a block(really it's smth else) faster then nonce wraps
		counter++;
	}
}
