#pragma once

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

	// max

	static Punum convert(float x); 
	static Punum convert(int x); 
	
	template<std::intmax_t N,std::intmax_t D>
	static constexpr Punum convert(std::ratio<N,D> r);

private:
	explicit Punum(int x) : v(x) {} // MAYBE private

};

#if 0
template <class T, int... exacts>
template<std::intmax_t N,std::intmax_t D>
constexpr Punum<T,exacts...> convert(std::ratio<N,D> x)
{
	if(x.num == 0)
		return zero();
	else if(x.num < 0)
		return -convert(td::ratio<-N,D>());
	int lo = 0;
	int hi = N;
	if (x.num < x.den)
	{
		while(true)
		{
            auto mid = lo + ((hi - lo) >> 1);
            if (mid == lo || mid == hi)
                break;
           	// inv(etable[mid]) > x
           	if(values[mid]*x.den > x.num)
           	{
	           	lo = mid;
	        }
           	else
           	{
           		hi = mid;
           	}
		}
        if (lo >= 0 && x.num == x.den*values[lo])
            return inv(fromexactsindex(lo)); // FIX
        else if (hi < N && x.num == x.den*values[hi]) // FIX as in the loop
            return inv(fromexactsindex(hi)); // FIX
        else if (lo == 0):
         return one().prev();
        else if hi >= N:
            return zero().next();
        else
            return inv((fromexactsindex(lo).next())); // FIX
	}
	else
	{
		while(true)
		{
            auto mid = lo + ((hi - lo) >> 1);
            if (mid == lo || mid == hi)
                break;
           	if(values[mid]*x.den < x.num)
           	{
	           	lo = mid;
	        }
           	else
           	{
           		hi = mid;
           	}
		}	
        if (lo >= 0 && (etable[lo]*x.den == x.num))
            return fromexactsindex(lo);
        else if (hi < N && (etable[hi]x.den == x.num))
            return (fromexactsindex(hi));
        else if (lo == 0):
         	return one().next();
        else if hi >= N:
            return inf().prev();
        else
            return fromexactsindex(lo).next();
	}
}
#endif


// functionals if needed
template <class T, int... exacts>
constexpr Punum<T,exacts...> neg(Punum<T,exacts...> x) { return -x; }

template <class T, int... exacts>
constexpr Punum<T,exacts...> inv(Punum<T,exacts...> x) { return ~x; }

// Bound object [first..last] or everything or empty
template <class APunum>
class Pbound
{
	APunum first,last;
	bool empty_ = true;

	Pbound() {}
	Pbound(APunum afirst, APunum alast): first(afirst),last(alast),empty_(false) {} 
	Pbound(APunum ax) : first(ax) ,last(ax),empty_(false) {}

	constexpr bool isempty() const  { return empty_; }
	constexpr bool iseverything() const { return !empty_ && first == last.next(); }
	constexpr bool issingle() const { return !empty_ && first == last; }
	constexpr bool isexact() const { return issingle() && first.isexact(); }
	constexpr bool iszero() const { return !empty_ && first.iszero(); }
	constexpr bool isone() const { return !empty_ && first.isone(); }
	constexpr bool isinf() const { return !empty_ && first.isinf(); }

	static constexpr Pbound zero()   { return Pbound(APunum::zero()); }
	static constexpr Pbound one()    { return Pbound(APunum::one()); }
	static constexpr Pbound inf()    { return Pbound(APunum::inf()); }
	static constexpr Pbound everything() { return Pbound(APunum::zero(),APunum::zero().prev()); }
	static constexpr Pbound empty() { return Pbound(); }
	constexpr Pbound inv() const { return isempty() ? *this : Pbound(~last,~first); }
	constexpr Pbound neg() const { return isempty() ? *this : Pbound(-last,-first); }
	constexpr Pbound complement() const { return empty_ ? everything() : iseverything() ? empty() : Pbound(last.next(),first.prev()); }

	constexpr Pbound operator-(const Pbound & other) const { return (*this) + (-other); }  // uses +
	constexpr Pbound operator/(const Pbound & other) const { return (*this) * other.inv(); } // uses *

	constexpr Pbound operator+(const Pbound & other) const { return *this; } // TBD
	constexpr Pbound operator*(const Pbound & other) const { return *this; } // TBD

	// in
	// intersect
	// shortestcover
	// outer when operation is CONVEX
	// finiteplus
	// +
	// *
	// ==
	// ^
	// exp
	// sqrt
	// maximum

};
