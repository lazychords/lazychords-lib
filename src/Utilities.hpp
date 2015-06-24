
#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include <string>
#include <sstream>
#include <vector>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/rational.hpp>
using Fraction = boost::rational<int>;
#pragma GCC diagnostic pop
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_free.hpp>
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


#endif // UTILITIES_HPP_INCLUDED
