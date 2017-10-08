#include <cstdint>
#include <iostream>
#include <ratio>

// T = support type (enough for 1<<N)
// exact = exact values ordered increasingly 
template <class T, int... exacts>
class Punum
{
public:
	enum { N = sizeof...(exacts), N2 =  N<<3, MASK = N2-1 }; 
	static constexpr int values[] = { exacts...};
	using index_t=T;

	T v; // index in the N2 space

	static constexpr Punum exactindex(int index) { return Punum((N2>>2)+(index<<1)); }; // index*2 + one being the first and zero-based

	constexpr Punum next() const { return Punum((v+1) & MASK ); }
	constexpr Punum prev() const { return Punum((v-1) & MASK ); }
	constexpr Punum abs()  const { return isstrictlynegative() ? -(*this): *this; }  // could be >= infinity because infinity is sign symmetric
	constexpr Punum neg()  const { return Punum(-v); }; 
	constexpr Punum inv()  const { return Punum(-(v+(N2 >> 1))); }
	constexpr bool isinf() const { return v == (N2>>1); }
	constexpr bool iszero() const { return v == 0; }
	constexpr bool isone() const { return v == (N2>>2); }
	constexpr bool isexact() const { return (v&1) == 0; }

	//C++14 constexpr bool isfractional() const { Punum w = abs(); return (w.v > 0) && (w.v < (N2>>2)); }
	constexpr bool isfractional() const { return v > 0 && (abs().v < (N2>>2)); } // (0 < x < 1) or  (-1 < x < 0) == (-1,1) removing 0
	constexpr bool isstrictlynegative() const { return v > (N2>>1); } // -inf < x < 0

	static constexpr Punum zero() { return Punum(0); }
	static constexpr Punum one() { return Punum(N2>>2); }
	static constexpr Punum inf() { return Punum(N2>>1); }

	// custom operators
	constexpr Punum operator-() const { return neg(); } 
	constexpr Punum operator~() const { return inv(); } 
	constexpr Punum operator-(const Punum & other) const { return (*this) + (-other); }  // uses +
	constexpr Punum operator/(const Punum & other) const { return (*this) * other.inv(); } // uses *

	constexpr Punum operator+(const Punum & other) const { return *this; } // TBD
	constexpr Punum operator*(const Punum & other) const { return *this; } // TBD

	// slowproduct

	// slowsum

	// exactvalue

	// iostream

	// sqrt

	// exp

	// conversion
	static Punum convert(float x); 
	static Punum convert(int x); 
	// MAYBE some fraction class or constant time rational from C++11
	//static Punum convert(rationa); // http://en.cppreference.com/w/cpp/numeric/ratio
private:
	explicit Punum(int x) : v(x) {} // MAYBE private

};

// functionals if needed
//template <class T, int... exacts>
//constexpr Punum<T,exacts...> neg(Punum<T,exacts...> x) { return x.neg(); }

// Bound object [first..last] or everything or empty
// TODO check being of the same unum
template <class APunum>
class Pbound
{
	APunum first,last;
	bool empty_ = true;

	Pbound() {}
	Pbound(APunum afirst, APunum alast): first(afirst),last(alast),empty_(false) {} 
	Pbound(APunum ax) : first(ax) ,last(ax),empty_(false) {}

	constexpr bool isempty() const  { return empty_; }
	constexpr bool iseverything() const;// TBD
	constexpr bool isone() const { return !empty_ && first == last; }

	static constexpr Pbound empty() { return Pbound(); }
	static constexpr Pbound everything() { return Pbound(APunum::zero(),APunum::zero().prev()); }

	// TODO: operations over the closed interval based on reduction

};

using X=Punum<uint8_t,1,2> ;


int main(int argc, char const *argv[])
{
	using Q=int [inv(X::one().next()/X::one()).v];
	std::cout << sizeof(X::values) << " " << X::N << " " << X::N2 << " " << X::MASK << " valuetest " << sizeof(Q) << std::endl;

	return 0;
}