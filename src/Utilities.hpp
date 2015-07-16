
#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include <string>
#include <sstream>
#include <vector>

#define GCC_IGNORE_WARNINGS \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wsign-conversion\"") \
_Pragma("GCC diagnostic ignored \"-Wconversion\"") \
_Pragma("GCC diagnostic ignored \"-Weffc++\"") \
_Pragma("GCC diagnostic ignored \"-Wshadow\"") \
_Pragma("GCC diagnostic ignored \"-Wundef\"") \
_Pragma("GCC diagnostic ignored \"-Wsuggest-attribute=noreturn\"") \
_Pragma("GCC diagnostic ignored \"-Wlogical-op\"")

#define GCC_PUT_BACK_WARNINGS _Pragma("GCC diagnostic pop")

GCC_IGNORE_WARNINGS
#include <boost/filesystem.hpp>
#include <boost/rational.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
GCC_PUT_BACK_WARNINGS

using Fraction = boost::rational<int>;

/**
 *@brief Puts a file into a string
 *@param str is the file name to read
 *@return the string in the file
**/
std::string getFile(const std::string& str);


template<typename T>
std::string toString(const T& a)
{
    std::ostringstream o;
    o<<a;
    return o.str();
}


//the following provides serialization mechanism for the Fraction type
namespace boost { namespace serialization {

    template <typename Archive, typename T>
        void save(Archive& ar, ::boost::rational<T> const& r, unsigned /*version*/)
        {
            int n = r.numerator(), d = r.denominator();
            ar & n;
            ar & d;
        }

    template <typename Archive, typename T>
        void load(Archive& ar, ::boost::rational<T>& r, unsigned /*version*/)
        {
            int n, d;
            ar & n;
            ar & d;
            r = ::boost::rational<T>(n, d);
        }

} }

BOOST_SERIALIZATION_SPLIT_FREE(boost::rational<int>)


template<unsigned B>
unsigned pow(unsigned p)
{
    unsigned x=1;
    for(unsigned i=0;i<p;i++)
        x*=p;
    return x;
}

template<unsigned B>
unsigned log(unsigned x)
{
    static_assert(B>1, "The base must be bigger than 1");
    unsigned p=0;
    while(x >= B)
    {
        x/=B;
        p++;
    }

    if(x!=1)
        throw std::runtime_error("x must be an exact power of the base. This is not the case (x = " + toString(x) + ", B="+toString(B));
    return p;
}
#endif // UTILITIES_HPP_INCLUDED
