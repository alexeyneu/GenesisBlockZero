#include <sys/socket.h>
#include <netdb.h>
#include <sstream>
#include <iostream>
#include <pqxx/pqxx>
#include <unistd.h>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <memory>


std::mutex com_mp;			// wait lock
std::condition_variable com_cvp;
std::atomic<bool> com_tr;

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
	do
	{
		pqxx::connection C("host=/run/postgresql");

		std::cout << "and then " << C.dbname() << std::endl;
		pqxx::work W{C};
		pqxx::result R{W.exec("TABLE russian_size")};
		for (auto row : R)
		{
			int lheaderend;
			int lb;
			int cnt;
			q = socket(AF_INET , SOCK_STREAM , 0);
			connect(q, (sockaddr *)&server, sizeof(server));
			std::stringstream f;
			f << "GET /v1/btc/test3/addrs/" << row[3] << "/balance HTTP/1.1\r\nhost:api.blockcypher.com\r\n\r\n";
			char b[1501];
			send(q, f.str().c_str(), f.str().length(), 0);
			cnt = recv(q, b, 1500, 0);
			close(q);
			b[cnt] = '\0';
			std::stringstream f2(b);
			lheaderend = f2.str().find("\r\n\r\n");
			lb = f2.str().find("\"final_balance\": ", lheaderend);
			f2.seekg(lb + std::string("\"final_balance\": ").length());
			long long cash = 0 , balance_btc_ex = 0;
			f2 >> cash;
			balance_btc_ex = row[6].as<long long>();
			if (cash > balance_btc_ex)
			{
				W.exec0("UPDATE russian_size SET f_balance_btc = " + pqxx::to_string(cash) + " WHERE id = " + row[0].c_str());
			}
			std::cout << b << std::endl << "length :" << cnt << std::endl << "cash in : " << cash << std::endl;


			f.str("");
			f.clear();
			q = socket(AF_INET , SOCK_STREAM , 0);
			connect(q, (sockaddr *)&server, sizeof(server));
			f << "GET /v1/eth/main/addrs/" << row[2] << "/balance HTTP/1.1\r\nhost:api.blockcypher.com\r\n\r\n";
			char bx[1501];
			send(q, f.str().c_str(), f.str().length(), 0);
			cnt = recv(q, bx, 1500, 0);
			close(q);
			bx[cnt] = '\0';
			f2.clear();
			f2.str(bx);
			lheaderend = f2.str().find("\r\n\r\n");
			lb = f2.str().find("\"final_balance\": ", lheaderend);
			f2.seekg(lb + std::string("\"final_balance\": ").length());
			cash = 0;
			int balance_eth_ex = 0;
			f2 >> cash;
			balance_eth_ex = row[7].as<long long>();
			if (cash > balance_eth_ex)
			{
				W.exec0("UPDATE russian_size SET f_balance_eth = " + pqxx::to_string(cash) + " WHERE id = " + row[0].c_str());
			}
			std::cout << bx << std::endl << "length :" << cnt << std::endl << "cash in : " << cash << std::endl;
		}
		W.commit();
		std::unique_lock<std::mutex> lk(com_mp);
		com_cvp.wait_for(lk ,std::chrono::milliseconds(54000) ,[] { return com_tr == !false; }); 
	} while(com_tr == false);
}