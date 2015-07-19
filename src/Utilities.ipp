#ifndef UTILITIES_IPP_H_INCLUDED
#define UTILITIES_IPP_H_INCLUDED

#include <type_traits>
#include "Concepts.hpp"
#include <chrono>

template<typename T>
std::string toString(const T& a)
{
    std::ostringstream o;
    o<<a;
    return o.str();
}

namespace boost { namespace serialization {

template <typename Archive, typename T>
void save(Archive& ar, ::boost::rational<T> const& r, unsigned /*version*/)
{
    T n = r.numerator(), d = r.denominator();
    ar & n;
    ar & d;
}

template <typename Archive, typename T>
void load(Archive& ar, ::boost::rational<T>& r, unsigned /*version*/)
{
    T n=1, d=1;
    ar & n;
    ar & d;
    r = ::boost::rational<T>(n, d);
}
}}

namespace log_impl{
//this is the implementation of the binary logarithm. Credit goes John Owens (http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog)
static constexpr unsigned int b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 
                                 0xFF00FF00, 0xFFFF0000};

static constexpr unsigned binary_log_impl(unsigned x,unsigned r, int i)
{
    return i<=0 ?
        r :
        binary_log_impl(x, r | safe_cast<unsigned>(((x & b[i]) != 0) << i) , i-1);
}
}
constexpr unsigned binary_log(unsigned x)
{
    return (x&(x-1))==0 ? //test if power of two
        log_impl::binary_log_impl(x,((x & log_impl::b[0]) != 0),4) : //computation
        throw std::runtime_error(toString(x)+" is not a power of two");
}

#endif // UTILITIES_IPP_H_INCLUDED
