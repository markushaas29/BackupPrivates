#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <filesystem>
#include "Transfers.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Unit/SIPrefix.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Common/DateTimes.hpp"

#ifndef TRANSFER_HPP
#define TRANSFER_HPP

namespace fs = std::filesystem;

namespace Bank
{
	struct In
	{
		using Type = In;
		inline static const std::string TypeId = "In"; 
		inline static constexpr int Id = 1; 
	};
	
	struct Out
	{
		using Type = Out;
		inline static const std::string TypeId = "Out"; 
		inline static constexpr int Id = -1; 
	};
	
	struct Unknown
	{
		using Type = Unknown;
		inline static const std::string TypeId = "Unknown"; 
		inline static constexpr int Id = 0; 
	};
	
	class DirectionBase: public Element
	{
	public:
		DirectionBase(std::string s): Element(s), value(Unknown::Id), id{Unknown::Id}, typeId{Unknown::TypeId} {};
		using Type = DirectionBase;
		using QuantityType = Quantity<Scalar,SIPrefix<0>>;
		inline static constexpr const char* Identifier = "Direction";
		DirectionBase* DoCreate(){return this;};
		const auto& Value() const {	return this->value; }
		const auto& Id() const  {	return this->id; }
		const auto& TypeId() const  {	return this->typeId; }	
	
	protected:
		std::string typeId = Unknown::TypeId; 
		int id = Unknown::Id; 		
		Quantity<Scalar,SIPrefix<0>,int> value;	
	private:
	};
	
	struct Direction: public DirectionBase
	{
		Direction(std::string s): DirectionBase(s){ this->template Set<Out>();};
		Direction(): DirectionBase(""){ };
		
		template<typename AccountT>
		void Update()
		{
			this->id = AccountT::Id;
			this->typeId = AccountT::TypeId;
			this->value = QuantityType(AccountT::Id);
			
			Logger::Log()<<"SET______"<<this->id<<"\t"<<this->typeId<<"\t"<<this->value<<std::endl;
		}
		
		template<typename DirectionT>
		void Set()
		{
			this->id = DirectionT::Id;
			this->typeId = DirectionT::TypeId;
			this->value = QuantityType(DirectionT::Id);
		}
	};
	
	//-----------------------------------------------------------------------------------------------Transfer-----------------------------------------------------------------------
	template<typename Account, typename TupleT>	class Transfer;
	
	template<typename ItemT, typename Account, typename TupleT>
	const ItemT& Get(Transfer<Account,TupleT>const& t)
	{
		return std::get<ItemT>(t.transferItems);
	};
	
	template<typename Account, typename TupleT>
	class Transfer
	{
	public:
		using CSVSeparator = T::char_<';'> ;
		using Type = Transfer<Account,TupleT> ;
		using TupleType = TupleT;
		using AccountType = Account ;
		using KeyType = Key<std::string>;
		using QunatityType = Quantity<Sum>;
		
		Transfer() = delete;
		Transfer(const TupleType& t) : transferItems(t) { 	};
		Transfer(TupleType&& t) : transferItems{std::move(t)} { 	};

		bool operator==(DayType d) const{ return std::get<DateTimes::Date>(transferItems) == d;};
		bool operator==(DateTimes::Day m) const{ return std::get<DateTimes::Date>(transferItems) == m;};
		bool operator==(MonthType m) const{ return std::get<DateTimes::Date>(transferItems) == m;};
		bool operator==(DateTimes::Month m) const{ return std::get<DateTimes::Date>(transferItems) == m;};
		bool operator==(YearType y) const{ return std::get<DateTimes::Date>(transferItems) == y;};
		bool operator==(DateTimes::Year y) const{ return std::get<DateTimes::Date>(transferItems) == y;};
		bool operator==(DateTimes::Date date) const{ return std::get<DateTimes::Date>(transferItems) == date;};
		bool operator==(DateType date) const{ return std::get<DateTimes::Date>(transferItems) == date;};
	private:
		template<typename ItemT, typename A, typename T>
		friend const ItemT& Get(Transfer<A,T>const& t);
		
		TupleType transferItems;
	};
}


#endif
