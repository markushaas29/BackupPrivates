#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../CSV/CSV.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Parser.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef COUNTER_H
#define COUNTER_H

template<typename Derived, typename U>
struct CounterType
{
	using Unit = U;
	using Type = Derived;
	static std::string Name;
};

struct Energy: public CounterType<Energy,Current>
{ 
	inline static std::string CounterType<Energy, Current>::Name = "Energy"; 
};

struct Water: public CounterType<Water,Volume>
{ 
	inline static std::string CounterType<Water, Volume>::Name = "Water"; 
};

struct Gas: public CounterType<Gas,Volume>
{ 
	inline static std::string CounterType<Gas, Volume>::Name = "Gas"; 
};


struct None
{
	inline static const std::string Value = "";
};
struct Hot
{
	inline static const std::string Value = "_Hot";
};
struct Cold
{
	inline static const std::string Value = "_Cold";
};

template<typename T>
struct AdditionalInformation
{
	inline static const std::string Value = T::Value;
};

template<typename MeterType,  int No = 0, typename U = typename MeterType::Unit, typename A = AdditionalInformation<None>>
struct CounterConfiguration
{
	static const uint Number = No;
	inline static const std::string AdditionalInformation = A::Value;
	inline static const std::string CounterName = String_::FromInt(No) + "_" + MeterType::Name + AdditionalInformation;
	inline static const std::string DestinataionPath = "//home//markus//Downloads//";
	using MeterT = MeterType;
	using Unit = U;
};

template<typename U, typename ValT = Value<double>, typename DateT = Date>
struct Reading
{
	using Unit = U;
	using ValueType = ValT;
	using DateType = DateT;
	const DateType Date;
	const ValueType Value;
	
	template<typename Separator = T::char_<';'>>
	void Display(std::ostream& out) const
	{
		out<<Date<<(char)Separator::Value<<Value<<std::endl;
	}
	
	Reading(ValueType val, DateType d): Date(d), Value(val)
	{
		Logger::Log()<<Value<<"\t"<<Date<<std::endl;
	}
};

template<typename C,typename T = double, typename DateT = Date>
std::ostream& operator<<(std::ostream& strm, const Reading<C,T,DateT> c)
{
	c.Display(strm);
	return strm;
}

template<typename ConfigT>
class Counter
{
private:
	using Config = ConfigT;
	using MeterType = Config::MeterT;
	using ReadingType = Reading<typename Config::Unit>;
	using ReadingContainerType = std::vector<ReadingType>;
	inline static const std::string DestinationPath = Config::DestinataionPath;
	inline static const std::string Name = Config::CounterName;
	
	static std::map<std::string, std::string> createHeader()
	{
		std::map<std::string, std::string> m;
		m["CounterName"] = Name;
		m["Number"] =  std::to_string(Config::Number),
		m["Type"] = MeterType::Name,
		m["SiUnit"] =  Config::Unit::SiUnit(),
		m["Unit"] =  Config::Unit::Sign();
        return m;
	}
	
	ReadingType CreateReading(std::vector<std::string> values)
	{
		auto date = Date(values.at(0));
		auto value = Value(values.at(1));
		
		return ReadingType(typename ReadingType::ValueType(value), typename ReadingType::DateType(date));
	}
	
	inline static const std::map<std::string, std::string> Header = createHeader();
	
	std::unique_ptr<ReadingContainerType> readings = std::unique_ptr<ReadingContainerType>(new ReadingContainerType());
	std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(DestinationPath + Name)));
	std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(this->fileInfo.get()));
	
public:	
	using Type = MeterType;
	using Unit = Config::Unit;
	inline static const uint Number = Config::Number;
	
	Counter(){}
	Counter(const Counter& c){}
	
	template<typename Separator = T::char_<'\t'>>
	void DisplayHeader(std::ostream& out) const
	{
		for (auto& it : Header)
			out<<it.first<<Separator::Value<<it.second<<std::endl;
	}
	
	template<typename Separator = T::char_<'\t'>>
	void Display(std::ostream& out) const
	{
		DisplayHeader(out);
		out<<std::endl;
		
		this->readings->push_back(ReadingType(9.0, Date("30.09.2021")));
		for(auto it = this->readings->cbegin(); it != this->readings->cend(); ++it)
			(*it).Display(out);
	}
	
	std::string GetName() const
	{
		return Name;
	}
	
	void Read()
	{
		auto values = csv->Read();
		
		for(int i = Header.size() + 1; i < values.size(); ++i)
			this->CreateReading(values.at(i));
	}
	
	void Write(const std::string sourcePath = ".")
	{
		csv->Write(*this);
	}
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const Counter<C> c)
{
	c.Display(strm);
	return strm;
}

using GasConfiguration = CounterConfiguration<Gas,1202757, Volume>;
using EnBWEnergyConfiguration = CounterConfiguration<Energy,21740069, Work>;
using VattenfallEnergyConfiguration = CounterConfiguration<Energy,11144078, Work>;
using AllWaterConfiguration = CounterConfiguration<Water,18061860, Volume>;
using OWaterConfiguration = CounterConfiguration<Water,279638, Volume>;
using OutWaterConfiguration = CounterConfiguration<Water,458711, Volume>;
using Bottom_HWaterConfiguration = CounterConfiguration<Water,15018324, Volume, AdditionalInformation<Hot>>;
using Bottom_CWaterConfiguration = CounterConfiguration<Water,15007241, Volume, AdditionalInformation<Cold>>;
using Middle_CWaterConfiguration = CounterConfiguration<Water,23267492, Volume, AdditionalInformation<Cold>>;
using Middle_HWaterConfiguration = CounterConfiguration<Water,14524889, Volume, AdditionalInformation<Hot>>;
using Top_CWaterConfiguration = CounterConfiguration<Water,25489823, Volume, AdditionalInformation<Cold>>;
using Top_HWaterConfiguration = CounterConfiguration<Water,25470737, Volume, AdditionalInformation<Hot>>;

#endif
