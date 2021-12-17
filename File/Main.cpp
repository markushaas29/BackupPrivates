#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include "Account.hpp"
#include "Repository.hpp"
#include "FileSystem.hpp"

using namespace FS;
//----------------------------------------------------------------------------PRODUCT----------------------------------------------------------
// int main()
// {
// 	
// 	std::vector<std::string> dirs= 
// 	{
// 		"/home/markus/Dokumente/cpp/Matrix",
// 		"/home/markus/Dokumente/cpp/File",
// 		"/home/markus/Dokumente/cpp/Visitor",
// 		"/home/markus/Dokumente/cpp/Unit",
// 		"/home/markus/Dokumente/cpp/Wrapper",
// 		"/home/markus/Dokumente/cpp/Traits",
// 		"/home/markus/Dokumente/cpp/SFINAE",
// 		"/home/markus/Dokumente/cpp/Quantity",
// 		"/home/markus/Dokumente/cpp/Logger",
// 		"/home/markus/Dokumente/cpp/Fraction",
// 		"/home/markus/Dokumente/cpp/CSV",
// 		"/home/markus/Dokumente/cpp/COW",
// 		"/home/markus/Dokumente/cpp/AbstractFactory",
// 		"/home/markus/Dokumente/cpp/Typelist",
// 		"/home/markus/Dokumente/cpp/String",
// 	};
// 	
// 	std::string to ="/media/markus/8591-1355/CPP/";
// // 	std::string to ="/home/markus/Downloads/";
// 	
// 	for(auto p : dirs)
// 	{
// 		std::string from = p;
// 		Backup::Repository::Backup(from,to);
// 	}
// 		
//     return 0;
// };

//----------------------------------------------------------------------------TEST----------------------------------------------------------


int main()
{
	std::ofstream outputfile ("//home//markus//Downloads//Output.txt");
	std::string fileName = "Unit.h";
	std::string custom = "RaibaKonten2021_1.csv";
	std::string comdirect = "Umsaetze_1026947527.csv";
	std::string raiba = "Umsaetze_DE19660623660009232702.csv";
	
	std::string csv = "/home/markus/Downloads/CSV_Files";
    CSV::Repository::Map(csv);

	auto c = CSV::Repository::Read(comdirect);	
	
    CSV::Repository::Attach();
    CSV::Repository::ParseAll();
    CSV::Repository::Display(std::cout);
	
	std::cout<<"\n TEST Transfers"<<std::endl;
	
	std::cout<<"\n-------------------------------------------------------- Transfers OUT --------------------------------------------------------------"<<std::endl;

	auto out = Transfers<Bank::Comdirect<0>>();
	auto com1 = out();
	com1.Display(outputfile);
	
	std::cout<<"\n-------------------------------------------------------- Transfers IN --------------------------------------------------------------"<<std::endl;
	auto in = Transfers<Bank::Comdirect<0>, Bank::In>();
	auto com2 = in();
	com2.Display(outputfile);

	std::cout<<"\n TEST GET Werner"<<std::endl;
	std::cout<<"\n-------------------------------------------------------- Transfers OUT --------------------------------------------------------------"<<std::endl;
	auto g = Get<Bank::Comdirect<0>>();
	auto gr = g(Key("Werner"));
	gr.Display(std::cout);
	
	std::cout<<"\n-------------------------------------------------------- Transfers IN --------------------------------------------------------------"<<std::endl;
	auto g2 = Get<Bank::Comdirect<0>, Bank::In>();
	auto gr2 = g2(Key("Werner"));
	gr2.Display(std::cout);
	
// 	for(auto i : com)
// 		std::cout<<*i<<Std::endl;
	
// 	auto ins = com::InTransfers()
// 	std::cout<<ins
	
// 	FS::Comdirect<0>::Transactions.Display(std::cout);
	
    return 0;
};
