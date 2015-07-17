
#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

/**
 *@file
 *@brief This file is used for functions/defines/typedefs that are used almost everywhere and do not have many similar functions.
 If many functions that can be grouped together are created, they will be merged to a separate file.
 *@author Julien
 *@todo Add GPL licence to the file
**/

#include <string>
#include <sstream>
#include <vector>

/**
 *@def GCC_IGNORE_WARNINGS
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
_Pragma("GCC diagnostic ignored \"-Wtype-limits\"") \
_Pragma("GCC diagnostic ignored \"-Wlogical-op\"")

/**
 *@def GCC_PUT_BACK_WARNINGS
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
 *@typedef UFraction = boost::rational<unsigned>
 *@brief We will be using the boost Fraction class for Fractions.
 *@note UFraction are unsigned Fractions.
 *@author Julien
**/
using UFraction = boost::rational<unsigned>;

/**
 *@fn std::string getFile(const std::string& str)
 *@brief Puts the content of a file into a string
 *@param str is the file name to read
 *@return the string representing the content of the file
 *@throw std::runtime_error("Could not read file") when the file described by str could not be opened.
 *@author Julien
 *@todo tests
**/
std::string getFile(const std::string& str);

/**
 *@fn template<typename T> std::string toString(const T& a)
 *@brief Tries to convert a type into a string using operator<<
 *@param a, the object to convert into a string
 *@pre T must have ostream& operator<<(ostream& const T& defined)
 *@return the string that represents the object
 *@author Julien
 *@todo tests
 *@todo check precondition through a concept
**/
template<typename T>
std::string toString(const T& a);


namespace boost { namespace serialization {

/**
 *@fn template <typename Archive, typename T> void save(Archive& ar, ::boost::rational<T> const& r, unsigned version)
 *@brief Serialization (saving part) for boost::rational<T>
 *@param ar is the archive in which we are saving the fraction
 *@param r is the fraction
 *@param version is the serialization version
 *@author Nicolas
 *@todo tests
 *@remark Shouldn't you add a precondition on T ? (Question from Julien)
**/
template <typename Archive, typename T>
void save(Archive& ar, ::boost::rational<T> const& r, unsigned version);

/**
 *@fn template <typename Archive, typename T> void load(Archive& ar, ::boost::rational<T>& r, unsigned version)
 *@brief Serialization (loading part) for boost::rational<T>
 *@param ar is the archive from which we are loading the fraction
 *@param r is the fraction
 *@param version is the serialization version
 *@author Nicolas
 *@todo tests
 *@remark Shouldn't you add a precondition on T ? (Question from Julien)
**/
template <typename Archive, typename T>
void load(Archive& ar, ::boost::rational<T>& r, unsigned version);

} }

/**
 *@internal
 *@brief Enables separate load and save functions for UFraction.
 *@remark Why separate load and save functions ? (Question from Julien)
 *@author Nicolas
**/
BOOST_SERIALIZATION_SPLIT_FREE(UFraction)

/**
 *@fn unsigned pow(unsigned base, unsigned power)
 *@brief pow function in for unsigned integers.
 *@note the function does not use quick exponentiation as p should be under 64 and in practice under 8.
 *@param power is the power
 *@param base is the base
 *@pre base^power must fit into an unsigned
 *@return base^power
 *@author Julien
 *@todo tests
**/
unsigned pow(unsigned base, unsigned power);

/**
 *@fn unsigned log(unsigned base, unsigned x)
 *@brief log function for unsigned integers.
 *@note the function uses naive logarithm algorithm
 *@param x is the number from which to take the log
 *@param base is the base.
 *@pre base must be strictly bigger than 1
 *@pre there must exist an integer p such that x = pow(base, p)
 *@note You should use another function if you want a truncated output
 *@return p such that x = pow(base, p)
 *@author Julien
 *@todo tests
**/
unsigned log(unsigned base, unsigned x);

/**
 *@fn template<typename T> T rand()
 *@brief returns a uniform random element of type T
 *@return a uniform random element of type T
 *@pre T must either be a primitive type or a type with randomInstance function
 *@note This function should not be used for cryptographic applications as the output is predictable.
 *@author Julien
 *@todo tests
**/
template<typename T>
T rand();

#include "Utilities.ipp"
#endif // UTILITIES_HPP_INCLUDED
