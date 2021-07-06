#include <iostream>
#include <pqxx/pqxx>

int main(int argc , char *argv[])
{
	pqxx::connection C("host=/run/postgresql");

	std::cout << "and then " << C.dbname() << std::endl;
	pqxx::work W{C};
	pqxx::result R{W.exec("TABLE russian_size")};
	for (auto row: R)
            std::cout << row[1].c_str() << std::endl;
}