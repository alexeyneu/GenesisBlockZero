#include <sys/socket.h>
#include <netdb.h> 
#include <sstream>
#include <iostream>
#include <pqxx/pqxx>
#include <unistd.h>


int main(int argc , char *argv[])
{
	int q;
  	hostent *host = gethostbyname("api.blockcypher.com"); 	
	sockaddr_in server = {AF_INET, htons( 80 ), *((unsigned long*)host->h_addr)};
/*	q = socket(AF_INET ,SOCK_STREAM ,0);
	connect(q, (sockaddr *)&server, sizeof(server));
	std::stringstream f;
	f << "GET /v1/btc/test3/addrs/tb1qyuq8v6n9cegxy020frqcg7dl4plte0g9awa7l9/balance HTTP/1.1\r\nhost:api.blockcypher.com\r\n\r\n"; 
	char b[5000] = {};
	send(q, f.str().c_str(), f.str().length(), 0);
	int cnt = recv(q, b, 5000, 0);
	close(q);
	std::cout << b  << std::endl << "length :" << cnt << std::endl;
*/
	pqxx::connection C("host=/run/postgresql");

	std::cout << "and then " << C.dbname() << std::endl;
	pqxx::work W{C};
	pqxx::result R{W.exec("TABLE russian_size")};
	for (auto row: R)
	{
    	std::cout << row[3] << std::endl;
		q = socket(AF_INET ,SOCK_STREAM ,0);
		connect(q, (sockaddr *)&server, sizeof(server));
		std::stringstream f;
		f << "GET /v1/btc/test3/addrs/" << row[3] << "/balance HTTP/1.1\r\nhost:api.blockcypher.com\r\n\r\n"; 
		char b[1500] = {};
		send(q, f.str().c_str(), f.str().length(), 0);
		int cnt = recv(q, b, 1500, 0);
		close(q);
		std::stringstream f2(b);
		int lheaderend = f2.str().find("\r\n\r\n");
		int lb = f2.str().find("\"final_balance\": ", lheaderend);
		f2.seekg(lb + std::string("\"final_balance\": ").length());
		int cash = 0;
		f2 >> cash;
		std::cout << b << std::endl << "length :" << cnt << std::endl << "cash in : " << cash << std::endl << std::endl;
	}
}