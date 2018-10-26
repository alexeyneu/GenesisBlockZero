#include <ctime>
#include <openssl/sha.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <iomanip>

#include <sstream>
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")

const uint32_t startNonce =2083236893; //0  ; 
const uint32_t unixtime =1231006505; //0 ;
const uint64_t COIN = 100000000;
const uint32_t OP_CHECKSIG = 172; // This is expressed as 0xAC
const uint8_t pubscriptlength = 67; // 2 + 65 ,  if unknown at compile time ... 
struct Transaction {
#pragma pack(1)
/* +0 */uint32_t version = 1;   	/* vs bitcoin-api tx block : merkleHash[32] removed */
	uint8_t  numInputs = 1; 
	uint8_t  prevOutput[32];
	uint32_t prevoutIndex = 0xFFFFFFFF; // +41
/* +0 */uint32_t sequence = 0xFFFFFFFF;
	uint8_t  numOutputs = 1; 
	uint64_t outValue = 50*COIN; 
	uint8_t  pubscriptlength = ::pubscriptlength; //it isn't in the bitcoin-api tx block also
	uint8_t  pubkeyScript[::pubscriptlength];
	uint32_t locktime = 0;   // +85
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
Transaction transaction; 
int main(int argc, char *argv[])
{
	unsigned char hash1[32], serializedData[857];
	uint32_t nBits = 0;
	if((argc-1) < 3) { std::cerr << "Usage: blockzero [options] <pubkey> \"<timestamp>\" <nBits>\n"; return -3; }
	if(strlen(argv[1])!=130) { std::cerr << "Invalid public key length! " << argv[1]; return -1; }
	if(strlen(argv[2]) > 254 || strlen(argv[2]) < 1) { std::cerr <<  "Size of timestamp is 0 or exceeds maximum length of 254 characters!\n"; return -2; }	
	if( sscanf(argv[3], "%d", (long unsigned int *)&nBits) || sscanf(argv[3], "%x", (long unsigned int *)&nBits)) ; 
	else { std::cerr << "where's nBits " << std::endl; return -1; } // short-circuit

	std::string pubkey(argv[1]) , timestamp(argv[2]);		
	transaction.pubkeyScript[0] = 0x41;
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
	unsigned char  block_hash1[32];
#ifdef _MSC_VER
		_declspec(align(16))	unsigned char  block_hashfp[32];
#else
		_attribute_((aligned(16))) unsigned char  block_hashfp[32];
#endif
		unsigned int counter=0, start = time(NULL);
		int drift = 4;
	__m128i r, m, b, camp;
	__m128i zero = _mm_setzero_si128();

	unsigned short turt;
	while(1) {

		SHA256((unsigned char*)&block_header, 80, block_hash1);
		SHA256(block_hash1, 32, block_hashfp);

		r = _mm_load_si128((__m128i *)block_hashfp);
		m = _mm_load_si128((__m128i *)(block_hashfp+16));
		switch (drift)
		{
			case 0:
			    break;
			case 1:
				b = _mm_alignr_epi8(m, r, 31);

			    break;
			case 2:
				b = _mm_alignr_epi8(m, r, 30);
		
			    break;
			case 3:
				b = _mm_alignr_epi8(m, r, 29);
			    
				break;
			case 4:
				b = _mm_alignr_epi8(m, r, 28);
		
			    break;
			case 5:
				b = _mm_alignr_epi8(m, r, 27);
		
			    break;
			case 6:
				b = _mm_alignr_epi8(m, r, 26);
		
			    break;
			case 7:
				b = _mm_alignr_epi8(m, r, 25);
		
			    break;
			case 8:
				b = _mm_alignr_epi8(m, r, 24);
	
			    break;
			case 9:
				b = _mm_alignr_epi8(m, r, 23);

			    break;
			case 10:

				b = _mm_alignr_epi8(m, r, 22);
			    break;
			case 11:
				b = _mm_alignr_epi8(m, r, 21);

			    break;
			case 12:
				b = _mm_alignr_epi8(m, r, 20);
		
			    break;
			case 13:
		
			    break;
			default:

			    break;
		}
/*now it turns out like:
* Hashes 6018c18fcd4bfabb523f2ee1f6c18880f38135f3eb0ff1aa76acbc2fa94eb8ce
* And 
* b		0000000000000000 000000006018c18f
*/

		camp = _mm_cmpeq_epi8(b ,zero);
		turt = _mm_movemask_epi8(camp);
		if (turt == 0xffff)
		{
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