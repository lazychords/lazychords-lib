
#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include <string>
#include <sstream>
#include <vector>

/**
 *@brief Disables g++ warnings, use it before including external libraries.
 Do not forget to use GCC_PUT_BACK_WARNINGS after including them.
 *@author Julien
**/
#define GCC_IGNORE_WARNINGS \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wsign-conversion\"") \
_Pragma("GCC diagnostic ignored \"-Wconversion\"") \
_Pragma("GCC diagnostic ignored \"-Weffc++\"") \
_Pragma("GCC diagnostic ignored \"-Wshadow\"") \
_Pragma("GCC diagnostic ignored \"-Wundef\"") \
_Pragma("GCC diagnostic ignored \"-Wsuggest-attribute=noreturn\"") \
_Pragma("GCC diagnostic ignored \"-Wlogical-op\"")

/**
 *@brief Reactivates g++ warnings after having used GCC_IGNORE_WARNINGS
 *@author Julien
**/
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

#include "Log.hpp"

/**
 *@brief We will be using the boost Fraction class for Fractions.
 *@note Fraction do not have to be positive, but will be used a such most of the time.
 *@author Julien
**/
using Fraction = boost::rational<int>;

/**
 *@brief Puts the content of a file into a string
 *@param str is the file name to read
 *@return the string representing the content of the file
 *@author Julien
**/
std::string getFile(const std::string& str);

/**
 *@brief Tries to convert a type into a string using operator<<
 *@param a, the object to convert into a string
 *@return the string that represents the object
 *@author Julien
**/
template<typename T>
std::string toString(const T& a);


namespace boost { namespace serialization {

/**
 *@brief Serialization (saving part) for boost::rational<T>
 *@param ar is the archive in which we are saving the fraction
 *@param r is the fraction
 *@param version is the serialization version
 *@author Nicolas
**/
template <typename Archive, typename T>
void save(Archive& ar, ::boost::rational<T> const& r, unsigned version);

/**
 *@brief Serialization (loading part) for boost::rational<T>
 *@param ar is the archive from which we are loading the fraction
 *@param r is the fraction
 *@param version is the serialization version
 *@author Nicolas
**/
template <typename Archive, typename T>
void load(Archive& ar, ::boost::rational<T>& r, unsigned version);

} }

/**
 *@brief What does this mean ?
 *@todo explain this
 *@author Nicolas
**/
BOOST_SERIALIZATION_SPLIT_FREE(boost::rational<int>)

/**
 *@brief pow function in for unsigned integers.
 *@note the function does not use quick exponentiation as p should be under 64 and in practice under 8.
 *@param power is the power
 *@param base is the base
 *@pre base^power must fit into an unsigned
 *@return base^power
 *@author Julien
 *@todo Check precondition with assert
**/
unsigned pow(unsigned base, unsigned power);

/**
 *@brief log function for unsigned integers.
 *@note the function uses naive logarithm algorithm
 *@param x is the number from which to take the log
 *@param base is the base.
 *@pre base must be strictly bigger than 1
 *@pre there must exist an integer p such that x = pow(base, p)
 *@note You should use another function if you want a truncated output
 *@return p such that x = pow(base, p)
 *@author Julien
**/
unsigned log(unsigned base, unsigned x);

/**
 *@brief returns a uniform random element of type T
 *@return a uniform random element of type T
 *@pre T must either be a primitive type or a type with randomInstance function
 *@note This function should not be used for cryptographic applications as the output is predictable.
 *@author Julien
**/
template<typename T>
T rand();

#include "Utilities.ipp"
#endif // UTILITIES_HPP_INCLUDED
