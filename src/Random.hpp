#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED
#include<random>
#include<limits>

class Random
{
public:
    /**@brief set the seed that must be used by all random generators 
     * @author alcinos
     * @param s the seed
     */
    static void setSeed(long unsigned s);

    /** @brief return a random integer between the given bounds. If no bounds are provided, the whole range of the integral type is used. 
     * @param a the lower bound
     * @param b the upper bound
     * @pre min(T)<=a<=b<=max(t)
     * @pre T is an integral type     
     * @author alcinos
     * @return a random number in [a,b]
     * @todo test
     */
    template<typename T >
    static T uniform_int(T a = std::numeric_limits<T>::lowest(),T b = std::numeric_limits<T>::max());
    
    /** @brief return a random real between the given bounds. If no bounds are provided, the number will be between 0 and 1. 
     * @param a the lower bound
     * @param b the upper bound
     * @pre a<=b
     * @pre T is an real type
     * @return a random number in [a,b]
     * @author alcinos
     * @todo test
     */
    template<typename T >
    static T uniform_real(T a = 0.0,T b = 1.0);

    /**@brief return a random boolean 
     * @return a random boolean
     */
    static bool random_bool();

    /**
     *@brief returns a uniform random element of type T
     *@return a uniform random element of type T
     *@pre T must either be a primitive type or a type with randomInstance function
     *@note This function should not be used for cryptographic applications as the output is predictable.
     *@author Julien & alcinos
     *@todo tests
     **/
    template<typename T>
    static T rand();

    
private:
    static void pickTimeSeed();
    static bool seedSet; ///< true if a seed has been chosen
    static long unsigned seed; ///< the seed used
    static std::mt19937 generator; ///< the PRG we use
};

#include "Random.ipp"
#endif
