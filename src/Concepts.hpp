
#ifndef CONCEPTS_HPP_INCLUDED
#define CONCEPTS_HPP_INCLUDED

#include "ConceptBase.hpp"

/**
 *@file
 *@brief toto
**/
ENABLE_CALLABLE_TEST(id)
ENABLE_CALLABLE_TEST(maxId)
ENABLE_CALLABLE_TEST(fromId)
ENABLE_CALLABLE_TEST(randomInstance)

namespace impl {}

namespace impl {
template<typename C, bool b>
struct HasIdImpl;

template<typename C, bool b>
struct HasRandomInstanceImpl;
}

/**
 *@namespace Concepts
 *@brief toto2
**/
namespace Concepts
{

/**
 *@struct template<typename C> struct HasId
 *@brief toto2
**/
template<typename C>
struct HasId
{
    static constexpr bool value = impl::HasIdImpl<C, std::is_class<C>::value>::value;

    /**
     *@anchor ConceptId
     *@brief If HasId<C>::value = true, checks if C::id, C::maxId, C::fromId respect the dynamic part of the concept id concept. Otherwise does not do anything.
     *@details Respecting the Id concept means :
     - From x = 0 to maxId -1 we have :
       -# fromId(x) never throws
       -# We have check(fromId(x))
       -# id(fromId(x)) never throws and is equal to x
     - For each randomInstance :
       -# id(randomInstance) does not throw
       -# id(randomInstance) is between 0 and maxId-1
     - For each x >= maxId, fromId throws
     *@throw never throws
     *@note this function is equivalent to a test.
    **/
    static void test();
};

template<typename C>
struct HasRandomInstance
{
    static constexpr bool value = impl::HasRandomInstanceImpl<C, std::is_class<C>::value>::value;
};
}

namespace impl {
template<typename C>
struct HasIdImpl<C, true>
{
    static constexpr bool value = IS_CALLABLE_MEMBER(id, int (C::*) ()) && IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && IS_CALLABLE_MEMBER(fromId, C (C::*) (int));
    static_assert(value || (!IS_CALLABLE_MEMBER(id, int (C::*) ()) && !IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && !IS_CALLABLE_MEMBER(fromId, C (C::*) (int))), "A class must either have all three functions id, maxId and fromId, or none");
};

template<typename C>
struct HasIdImpl<C, false>
{
    static constexpr bool value = false;
};

template<typename C>
struct HasRandomInstanceImpl<C, true>
{
    static constexpr bool value = IS_CALLABLE_MEMBER(randomInstance, C (C::*) ());
};

template<typename C>
struct HasRandomInstanceImpl<C, false>
{
    static constexpr bool value = false;
};

}
#endif // CONCEPTS_HPP_INCLUDED

