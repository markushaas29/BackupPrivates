#include <iostream>
#include <boost/mpl/vector.hpp>
#include "UnitRatio.hpp"
#include "Counter.hpp"
#include "../Tuple/Tuple.hpp"
#include "CounterContainer.hpp"
#include "../Traits/Traits.h"
#include "../Unit/Unit.h"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	using W1 = CounterConfiguration<Water,1>;
	using G1 = CounterConfiguration<Gas,1>;
	using E1 = CounterConfiguration<Energy,1>;
	using W2 = CounterConfiguration<Water,2>;
	using W3 = CounterConfiguration<Water,3>;
	
	using CW1 = Counter<W1>;
	using CE1 = Counter<E1>;
	using CG1 = Counter<G1>;
	using CW2 = Counter<W2>;
	using CW3 = Counter<W3>;
	
	using TypeContainer = Tuple<CW1,CW2,CW3,CE1,CG1>;
		
	auto tc = TypeContainer();
	auto cw3 = Get<2>(tc);
	cw3.Display(std::cout);
	
	auto cw3T = Get<CW3>(tc);
	cw3T.Display(std::cout);
	
	CounterContainer<CW1,CW2,CW3,CE1,CG1> cc;
	cc.Display(std::cout);
	
	auto z = cc.GetCounter<1>();
	z.Display(std::cout);
	
	auto zT = cc.GetCounter<CW2>();
	zT.Display(std::cout);
	
	using s = Sum;
	using work = Work;
	auto cw = CW1("W1"); 
		
    std::cout<<"Sum "<<Sum::UnitSign()<<std::endl;
    std::cout<<"Work "<<Work::UnitSign()<<std::endl;
	
    std::cout<<"Type "<<CW1::Type::Name<<std::endl;
    std::cout<<"Unit "<<CW1::Unit::UnitSign()<<std::endl;
    std::cout<<"Name "<<cw.Name()<<std::endl;
    std::cout<<"F "<<Kilo::Type::Den<<std::endl;
    std::cout<<"F "<<Kilo::Type::Num<<std::endl;
    std::cout<<"F "<<Kilo::Sign<<std::endl;
    std::cout<<"END"<<std::endl;

    return 0;
}
