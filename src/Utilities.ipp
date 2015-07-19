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

namespace impl
{

inline decltype(std::chrono::high_resolution_clock::now().time_since_epoch().count()) getSeed()
{
    auto res = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    Log::debugInfo("Seed used is : " + toString(res), __FILE__, __LINE__);
    return res;
}

template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
T rand()
{
    static auto seed = getSeed();
    static std::mt19937 gen(static_cast<std::mt19937::result_type>(seed));
    std::uniform_int_distribution<T> uniform;
    T r =  uniform(gen);
    return r;
}

template<typename T, typename = typename std::enable_if<!std::is_integral<T>::value>::type, typename = void>
T rand()
{
    T r = T::randomInstance();
    return r;
}
}


template<typename T>
T rand()
{
    static_assert(Concepts::HasRandomInstance<T>::value || std::is_integral<T>::value, "You can only generate random instances of functions with randomInstance implemented or for integral constants");
    return impl::rand<T>();
}
#endif // UTILITIES_IPP_H_INCLUDED
