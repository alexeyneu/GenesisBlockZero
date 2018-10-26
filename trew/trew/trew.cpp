
#include "stdafx.h"
#include "certs.h"
#include "C:\Users\alex\Documents\New folder\openssl\ms\applink.c"

const	std::string bin2hex(const unsigned char *p, size_t length)
{
	std::stringstream f;
	f << std::hex << std::setfill('0');
	for (int i = 0; i < length; i++) f << std::setw(2) << (int)p[i];
	return f.str();
}




	HANDLE cl;

UINT CALLBACK mainthread(void *h1);
_declspec(align(16)) struct triggerblock
{
	double q;
	long long block[3];
	long long b;
	long long t;
	tm *c;
	tm *p;
	BYTE ptrigger;
	float outofthis;
	double x;
	long double F;
	short finishup;
	double f;
	BYTE E;
	short pl = 7;
};
enum torc{
	HELO,
	ERR,
	POW,
	END,
	NAME,
	MAILNUM,
	MAIL1,
	MAIL2,
	SKYPE,
	BIRTHDATE,
	COUNTRY,
	ADDRNUM,
	ADDRLINE1,
	ADDRLINE2
};
typedef std::map<std::string, torc> braze;
std::string rk;
triggerblock mo;
std::string f;
UINT CALLBACK stonehill(void *h1);
int wmain(int argc, wchar_t* argv[])
{
	cl = CreateEvent(NULL, 1, 0, NULL);
	braze	row;
	row.insert(braze::value_type("HELO", HELO));
	row.insert(braze::value_type("ERROR", ERR));
	row.insert(braze::value_type("POW", POW));
	row.insert(braze::value_type("END", END));
	row.insert(braze::value_type("NAME", NAME));
	row.insert(braze::value_type("MAILNUM", MAILNUM));
	row.insert(braze::value_type("MAIL1", MAIL1));
	row.insert(braze::value_type("MAIL2", MAIL2));
	row.insert(braze::value_type("SKYPE", SKYPE));
	row.insert(braze::value_type("BIRTHDATE", BIRTHDATE));
	row.insert(braze::value_type("COUNTRY", COUNTRY));
	row.insert(braze::value_type("ADDRNUM", ADDRNUM));
	row.insert(braze::value_type("ADDRLINE1", ADDRLINE1));
	row.insert(braze::value_type("ADDRLINE2", ADDRLINE2));

	WSADATA fc;
	WSAStartup(0x202, &fc); //sockets 2.02 ,hibyte and lobyte = 2 
	hostent *host = gethostbyname("34.245.228.255");
	sockaddr_in server = { AF_INET, htons(65535) /* btx port */, *(in_addr*)(unsigned long*)host->h_addr };
	SOCKET q = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	BOOL c = 1;
	char  sr[500] = {};
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	ERR_load_BIO_strings();

	SSL_CTX *sslctx = SSL_CTX_new(TLSv1_2_client_method());
	SSL_CTX_set_timeout(sslctx, 7200);
	SSL_CTX_set_options(sslctx, SSL_OP_NO_SSLv2);
	RAND_poll();

	X509 *cab = NULL;
	X509 *cert_w = NULL;
	RSA *keyt = NULL;
	BIO *cbio, *kbio, *cabio;
	cbio = BIO_new_mem_buf(cert.c_str(), -1);
	cert_w = PEM_read_bio_X509(cbio, NULL, 0, NULL);

	cabio = BIO_new_mem_buf(cacert.c_str(), -1);
	cab = PEM_read_bio_X509(cabio, NULL, 0, NULL);

	kbio = BIO_new_mem_buf(key.c_str(), -1);
	keyt = PEM_read_bio_RSAPrivateKey(kbio, NULL, 0, NULL);
	HANDLE t[75];
	UINT t7 = 7200000;
	SSL_CTX_use_certificate(sslctx, cert_w);
	SSL_CTX_add_extra_chain_cert(sslctx, cab);

	mo.b = SSL_CTX_use_RSAPrivateKey(sslctx, keyt);
	std::cerr << mo.b << std::endl;
	SSL *cSSL = SSL_new(sslctx);
	setsockopt(q, SOL_SOCKET, SO_KEEPALIVE, (char *)&c, sizeof(c));
	setsockopt(q, SOL_SOCKET, SO_SNDTIMEO, (char *)&t7, sizeof(t7));
	setsockopt(q, SOL_SOCKET, SO_RCVTIMEO, (char *)&t7, sizeof(t7));
	setsockopt(q, IPPROTO_TCP, TCP_NODELAY, (char *)&c, sizeof(c));

	BIO *u7 = BIO_new_socket(q, BIO_NOCLOSE);

	connect(q, (sockaddr*)&server, sizeof(server));
	int timeout = 2980000;  // user timeout in milliseconds [ms]
	setsockopt(q, 6, 18, (char*)&timeout, sizeof(timeout));


	SSL_set_bio(cSSL, u7, u7);
	mo.b = SSL_CTX_check_private_key(sslctx);
	std::cerr << mo.b << std::endl;


	SSL_set_tlsext_host_name(cSSL, "34.245.228.255");
	SSL_set_connect_state(cSSL);
	mo.finishup = SSL_connect(cSSL);
  	if (!(mo.finishup > 0))	{ ERR_print_errors_fp(stderr); return 0; }



	while (mo.finishup != WAIT_TIMEOUT)
	{
		std::string s;
		std::stringstream fr;
		std::string c;
		std::string hastebase;
		BYTE tons[20];

		mo.t = SSL_read(cSSL, sr, 740);
		fr.write(sr, mo.t);
		int zx{};
		fr >> c;
		std::cerr << c << " :";
		if (c == "") {
			std::cerr << " damage case";
			break;
		}
		switch (row[c])
		{
		case HELO:
			s = "EHLO\n";
//			SSL_heartbeat(cSSL);

			SSL_write(cSSL, s.c_str(), s.length());
			break;
		case ERR:
			mo.finishup = WAIT_TIMEOUT;
			std::cerr << fr.str() << std::endl;
			mo.finishup = WAIT_TIMEOUT;

			break;
		case POW:
			fr >> f >> mo.pl;
			std::cerr << fr.str() << std::endl;
			t[zx++] = (HANDLE)_beginthreadex(NULL, 0, mainthread, (LPVOID)zx, 0, NULL);
			t[zx++] = (HANDLE)_beginthreadex(NULL, 0, mainthread, (LPVOID)zx, 0, NULL);
			t[zx++] = (HANDLE)_beginthreadex(NULL, 0, mainthread, (LPVOID)zx, 0, NULL);
			t[zx++] = (HANDLE)_beginthreadex(NULL, 0, mainthread, (LPVOID)zx, 0, NULL);
			t[40] = (HANDLE)_beginthreadex(NULL, 0, stonehill, (LPVOID)cSSL, 0, NULL);
			mo.finishup = WaitForMultipleObjects(zx, t, 0, 7150000);
			zx = 0;
			if (zx++ != mo.finishup - WAIT_OBJECT_0)TerminateThread(t[zx-1], 0);
			if (zx++ != mo.finishup - WAIT_OBJECT_0)TerminateThread(t[zx-1], 0);
			if (zx++ != mo.finishup - WAIT_OBJECT_0)TerminateThread(t[zx-1], 0);
			if (zx++ != mo.finishup - WAIT_OBJECT_0)TerminateThread(t[zx-1], 0);
			SetEvent(cl);
			WaitForSingleObject( t[40], 2313);
			
//			rk = rk + '\n';
//			SSL_write(cSSL, rk.data(), rk.size());
//			std::cerr << rk.c_str() << std::endl;

			break;
		case END:
			s = "OK\n";
			SSL_write(cSSL, s.c_str(), s.length());
			std::cerr << "REST " << std::endl;
			mo.finishup = WAIT_TIMEOUT;

			break;
		case NAME:
			fr >> hastebase;
			SHA1((BYTE *)(f + hastebase).data(), (f + hastebase).size(), tons);
//			std::reverse(tons, tons + 20);
			s = bin2hex(tons, 20) + " " + "Alexey Neudatchin" + "\n";
			SSL_write(cSSL, s.data(), s.size());

			break;
		case MAILNUM:
			fr >> hastebase;
			SHA1((BYTE *)(f + hastebase).data(), (f + hastebase).size(), tons);
//			std::reverse(tons, tons + 20);
			s = bin2hex(tons, 20) + " " + "1\n";
			SSL_write(cSSL, s.data(), s.size());

			break;
		case MAIL1:
			fr >> hastebase;
			SHA1((BYTE *)(f + hastebase).data(), (f + hastebase).size(), tons);
//			std::reverse(tons, tons + 20);
			s = bin2hex(tons, 20) + " " + "alexeyneu@zoho.eu\n";
			SSL_write(cSSL, s.data(), s.size());

			break;
		case MAIL2:

			break;
		case SKYPE:
			fr >> hastebase;
			SHA1((BYTE *)(f + hastebase).data(), (f + hastebase).size(), tons);
//			std::reverse(tons, tons + 20);
			s = bin2hex(tons, 20) + " " + "alexeyneu@zoho.eu\n";
			SSL_write(cSSL, s.data(), s.size());

			break;
		case BIRTHDATE:
			fr >> hastebase;
			SHA1((BYTE *)(f + hastebase).data(), (f + hastebase).size(), tons);
//			std::reverse(tons, tons + 20);
			s = bin2hex(tons, 20) + " " + "20..\n";
	SSL_write(cSSL, s.data(), s.size());

			break;
		case COUNTRY:
			fr >> hastebase;
			SHA1((BYTE *)(f + hastebase).data(), (f + hastebase).size(), tons);
//			std::reverse(tons, tons + 20);
			s = bin2hex(tons, 20) + " " + "Russia\n";
			SSL_write(cSSL, s.data(), s.size());

			break;
		case ADDRNUM:
			fr >> hastebase;
			SHA1((BYTE *)(f + hastebase).data(), (f + hastebase).size(), tons);
//			std::reverse(tons, tons + 20);
			s = bin2hex(tons, 20) + " " + "1\n";
			SSL_write(cSSL, s.data(), s.size());

			break;
		case ADDRLINE1:
			fr >> hastebase;
			SHA1((BYTE *)(f + hastebase).data(), (f + hastebase).size(), tons);
//			std::reverse(tons, tons + 20);
			s = bin2hex(tons, 20) + " " + "Moscow Region ,  84\n";
			SSL_write(cSSL, s.data(), s.size());

			break;
		case ADDRLINE2:

			break;
		default:
			mo.finishup = WAIT_TIMEOUT;
			break;
		}


	};
	SSL_shutdown(cSSL);
	SSL_free(cSSL);
	SSL_CTX_free(sslctx);
	closesocket(q);
	WSACleanup();

	return 0;

}
UINT CALLBACK stonehill(void *h1)
{
		SSL *cSSL = (SSL*)h1;
		DWORD c{};
while (c == WAIT_TIMEOUT)
	{

		SSL_heartbeat(cSSL);
		c = WaitForSingleObject(cl,100000);
//		if (SSL_get_tlsext_heartbeat_pending(cSSL)) 	std::cout << " no bowler, ";
	}  
			rk = rk + '\n';
			SSL_write(cSSL, rk.data(), rk.size());
			std::cerr << rk.c_str() << std::endl;
	
	return 0;

}
UINT CALLBACK mainthread(void *h1)
{

	const int x = 16;
	//	_declspec(align(64)) BYTE bn[x] = {};
	int cx = Base64EncodeGetRequiredLength(x), hi = cx;
	std::stringstream tr(f);
	BYTE u = tr.str().length();
	BYTE ox = u + cx;
	char *q;
	q = (char *)_aligned_malloc(ox, 64);
	tr.str().copy((char *)q, u);
	int dist = mo.pl / 2;
	_declspec(align(64)) BYTE hash[32] = {};

//	RAND_poll();
	__m128i r, m, b, camp;
	__m128i zero = _mm_setzero_si128();

	USHORT turt;

	_declspec(align(64))	union tobbi{ BYTE bn[x]; unsigned long long int er[2]; };
	tobbi w = {};
	RAND_bytes(w.bn, x);

	while (1)
	{
		cx = hi;
		Base64Encode(w.bn, x, q + u, &cx, ATL_BASE64_FLAG_NOCRLF | ATL_BASE64_FLAG_NOPAD);
		ox = u + cx;

		//		EVP_DigestInit_ex(cont,EVP_sha1(),NULL);
		//		EVP_DigestUpdate(cont, (unsigned char*)(BYTE *)q, ox);
		//		EVP_DigestFinal_ex(cont, hash, &rx);


		SHA1((BYTE *)q, ox, hash);
		r = _mm_load_si128((const __m128i *)hash);
//		m = _mm_load_si128((const __m128i *)(hash + 16));

		switch (dist)
		{
		case 0:
			b = _mm_slli_si128(r, 15);
			break;
		case 1:
			b = _mm_slli_si128(r, 15);

			break;
		case 2:
			b = _mm_slli_si128(r, 14);

			break;
		case 3:
			b = _mm_slli_si128(r, 13);
			break;
		case 4:
			b = _mm_slli_si128(r, 12);

			break;
		case 5:
			b = _mm_slli_si128(r, 11);

			break;
		case 6:
			b = _mm_slli_si128(r, 10);

			break;
		case 7:
			b = _mm_slli_si128(r, 9);

			break;
		case 8:
			b = _mm_slli_si128(r, 8);

			break;
		case 9:
			b = _mm_slli_si128(r, 7);

			break;
		case 10:
			b = _mm_slli_si128(r, 6);

			break;
		case 11:
			b = _mm_slli_si128(r, 5);

			break;
		case 12:

			break;
		case 13:

			break;
		default:
			break;
		}
		camp = _mm_cmpeq_epi8(b, zero);
		turt = _mm_movemask_epi8(camp);
		if (turt == 0xffff)
		{
			std::reverse(hash, hash + 20);
			std::cout << " Hashes " << bin2hex(hash, 20) << std::endl;
			std::reverse(hash, hash + 20);
			if ((hash[dist + mo.pl % 2 - 1] & 0xf0) == 0)
			{
				rk.assign(&q[u], &q[ox]);
				std::cout << '\007' << std::endl;
				break;
			}
		}
		if (++w.er[0] == 0 && ++w.er[1] == 0) RAND_bytes(w.bn, x);

	}
	_aligned_free(q);

	//	EVP_MD_CTX_destroy(cont);

	return 0;
}
