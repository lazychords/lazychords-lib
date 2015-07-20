#ifndef RANDOM_IPP_INCLUDED
#define RANDOM_IPP_INCLUDED
#include "Concepts.hpp"
#include "Utilities.hpp"
template<typename T>
T Random::uniform_int(T a ,T b)
{
    static_assert(std::is_integral<T>::value,"Uniform integer generation requires an integral type");
    ASSERT(a<=b);
    ASSERT(std::numeric_limits<T>::lowest()<=a);
    ASSERT(b<=std::numeric_limits<T>::max());
    if(!seedSet){
        pickTimeSeed();
    }
    std::uniform_int_distribution<T> u(a,b);
    return u(generator);

}

template<typename T>
T Random::uniform_real(T a ,T b)
{
    static_assert(std::is_floating_point<T>::value,"Uniform integer generation requires an real number type");
    ASSERT(a<=b);
    ASSERT(std::numeric_limits<T>::lowest()<=a);
    ASSERT(b<=std::numeric_limits<T>::max());
    if(!seedSet){
        pickTimeSeed();
    }
    std::uniform_real_distribution<T> u(a,b);
    return u(generator);

}


namespace rand_impl
{
template<typename T,typename = typename std::enable_if<std::is_integral<T>::value && !std::is_same<T,bool>::value>::type>
T rand()
{
    T elem = Random::uniform_int<T>();
    return elem;
}
template<typename T,typename = typename std::enable_if<std::is_floating_point<T>::value>::type,typename = void>
T rand()
{
    T elem = Random::uniform_real<T>();
    return elem;
}
template<typename T,typename = typename std::enable_if<Concepts::HasRandomInstance<T>::value>::type,typename = void, typename = void>
T rand()
{
    return T::randomInstance();
}

template<typename T,typename = typename std::enable_if<std::is_same<T, bool>::value>::type, typename = void, typename = void, typename = void>
T rand()
{
    return Random::rand<unsigned>()%2;
}
}

template<typename T>
T Random::rand()
{
    static_assert(Concepts::HasRandomInstance<T>::value || std::is_integral<T>::value || std::is_floating_point<T>::value, "You can only generate random instances of functions with randomInstance implemented or for integral/real constants");
    return rand_impl::rand<T>();
}

#endif
