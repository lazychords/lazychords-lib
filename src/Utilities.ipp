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

#endif // UTILITIES_IPP_H_INCLUDED
