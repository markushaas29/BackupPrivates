#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "Info.hpp"

#ifndef FILETYPECONTAINER_HPP
#define FILETYPECONTAINER_HPP

namespace FS
{
	//---------------------------------------------------------------------------------------------------FileTypeContainer----------------------------------------------------------------------------------------
	template<typename List>
	class FileTypeContainer	{};
	
	template<typename Head>
	class FileTypeContainer<Typelist<Head>>
	{
	protected:
		std::string rootPath;
		std::filesystem::path BuildDestPath(const std::string& src, const std::string &dst)
		{
			auto srcPath = std::filesystem::path(src);
			auto rootP = std::filesystem::path(rootPath);
			auto dstPath = std::filesystem::path(dst);
			std::string result = "";

			auto it2 = rootP.begin();

			for (auto it = --srcPath.end() ; it != srcPath.begin(); --it)
			{
// 					result = result + "/" + (*it).string();
					
				if(*it == *(--rootP.end()))
				{
					result = (*it).string() + "/" + result ;
					break;
				}
				
				result = (*it).string() + "/" + result ;
				Logger::Log()<<"BuildDestPath: "<<*it<<"=>"<<result<<std::endl;
// 				if((*(--rootP.end())) == *it || *it != *it2)
			}
			
			Logger::Log()<<"BuildDestPath: "<<dst<<"-----------"<<result<<std::endl;
			result = dst + result;
			
			Logger::Log()<<"BuildDestPath: "<<result<<std::endl;
			
			return std::filesystem::path(std::string(result.cbegin(),(--result.cend())));
		}
	public:
		void SetRootPath(std::string p){ this->rootPath = p;}
		using Type = Head;
		
		void Add(FileInfo* fi)
		{
			if(strcmp(Type::Extension, fi->Extension()) == 0)
				Head::Add(fi); 
		}
		
		void CopyTo(std::string dest)
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{
				std::string dst = this->BuildDestPath(it->Info().Path(),dest);
				it->CopyTo(dst);
			}
		}
		
		void List()
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{				
				Logger::Log()<<it->Info()<<std::endl;
			}			
		}
		
		FileTypeContainer()	{ }
	};
	
	template<typename Head, typename... Tail>
	class FileTypeContainer<Typelist<Head,Tail...>>: public FileTypeContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;

		void Add(FileInfo* fi)
		{
			if(strcmp(Type::Extension, fi->Extension()) == 0)
			{
				Head::Add(fi); 
			}
			else
			{
				FileTypeContainer<Typelist<Tail...>>::Add(fi);
			}				
		}
		
		void CopyTo(std::string dest)
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{
				std::string dst = this->BuildDestPath(it->Info().Path(),dest);
				Logger::Log()<<"Destination: "<<dst<<std::endl;
				it->CopyTo(dst);
			}
			
			FileTypeContainer<Typelist<Tail...>>::CopyTo(dest);
		}
		
		void List()
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{				
				Logger::Log()<<it->Info()<<std::endl;
			}
			
			FileTypeContainer<Typelist<Tail...>>::List();
		}
		
		FileTypeContainer() { };
	};

}


#endif
