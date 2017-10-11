#include "unum2.hpp"


using X=Punum<uint8_t,1,2> ;


int main(int argc, char const *argv[])
{
	using Q=int [inv(X::one().next()/X::one()).v];
	std::cout << sizeof(X::values) << " " << X::N << " " << X::N2 << " " << X::MASK << " valuetest " << sizeof(Q) << std::endl;

	std::cout << sizeof(Pbound<X>) << std::endl;
	return 0;
}