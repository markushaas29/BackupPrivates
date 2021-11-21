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
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <filesystem>
#include "Account.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"

#ifndef CUSTOM_HPP
#define CUSTOM_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<unsigned int N = 0>
	struct Custom: public Account<Custom<N>>
	{
		enum{ Num = N };
		using InType = AccountTransfer<Custom,Transfer<In>>;
		using OutType = AccountTransfer<Custom,Transfer<Out>>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Custom>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Custom";
		inline static const std::string Filename = "RaibaKonten2021_1.csv";
		inline static constexpr unsigned int OwnerIdx = 1;
		inline static constexpr unsigned int TranactionIdx = 2;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 3;
		inline static constexpr unsigned int HeaderLength = 15;
		inline static constexpr unsigned int TrailerLength = 4;
		
		inline static Base::ParseContIn InCont = typename Base::ParseContIn();
		inline static Base::ParseContOut OutCont = typename Base::ParseContOut();
		Custom(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};		
		
		static void Display(std::ostream& os)
		{
			InCont.Display(os);
		}
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			return s;
		}
		
		static std::string ExtractKey(std::string s)
		{
			return s;
		}
		
		static void ProcessValues(std::vector<std::string> values)
		{
			auto keyLine = values.at(OwnerIdx);
			if(keyLine != "")
			{
				auto key = ExtractKey(keyLine);
				auto date = values.at(DateIdx);
				auto transaction = values.at(TranactionIdx);
				
				auto n = Base::GetNumericValue(values.at(QuantityIdx));
				auto sum = n != "" ? std::stod(n) : 0.0 ;
			
				auto iban =  Extract<IBAN>(transaction);
				auto bic = Extract<BIC>(transaction);

				InCont.Insert(key, typename Base::InTransfer(key,transaction,sum, date, iban, bic));
				OutCont.Insert(key, typename Base::OutTransfer(key,transaction,sum, date, iban, bic));
				
				Base::InsertInContainer(key,transaction,sum, date, iban, bic, *(values.at(QuantityIdx).begin()+1));
			}
		}				
				

	};
}


#endif
