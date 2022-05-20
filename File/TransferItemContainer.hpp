#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "../CSV/CSV.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"

#ifndef TRANSFERCONTAINER_HPP
#define TRANSFERCONTAINER_HPP

//---------------------------------------------------------------------------------------------------CounterContainer----------------------------------------------------------------------------------------

template<typename List>
class TransferItemContainer{};

template<typename Head>
class TransferItemContainer<Typelist<Head>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head>;
	using ContainerType = TransferItemContainer<Typelist<Head>>;
protected:
	TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return Type::Display(os);
	}
		
	void Read(const std::string sourcePath = ".")
	{
		Logger::Log()<<Type::Identifier<<std::endl;
	}
	
	template<unsigned N>
	auto Get() { return At<CounterTypes,N>::Type; }

	static TransferItemContainer& Instance()
	{
		static TransferItemContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
class TransferItemContainer<Typelist<Head,Tail...>>: public TransferItemContainer<Typelist<Tail...>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head,Tail...>;
	using ContainerType = TransferItemContainer<Typelist<Head,Tail...>>;
	using Base = TransferItemContainer<Typelist<Tail...>>;
protected:
	TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return Base::Display(Type::Display(os));		
	}
		
	void Read(const std::string sourcePath = ".")
	{
		Logger::Log()<<Type::Identifier<<std::endl;
		Base::Read();		
	}
	
	template<unsigned N>
	auto Get() { return At<CounterTypes,N>::Type; }

	static TransferItemContainer& Instance()
	{
		static TransferItemContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const TransferItemContainer<Head,Tail...> c)
{
	return c.Display(strm);
}

using TransferItemContainerType = TransferItemContainer<Typelist<IBAN,BIC>>::ContainerType;

#endif
