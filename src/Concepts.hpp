
#ifndef CONCEPTS_HPP_INCLUDED
#define CONCEPTS_HPP_INCLUDED


#define IS_CALLABLE_MEMBER(F, Type) impl::Callable_##F<Type>::value



namespace Concepts
{
template<typename C>
struct IsCheckable;

template<typename C>
struct HasId;

template<typename C>
struct HasRandomInstance;
}

#include "Concepts.ipp"

/**
 *@namespace Concepts
 *@brief This namespace is used for Concepts, that it to say for checking if types follow the desired behavior.
 *@details Here is the Concept implication graph
 *  @dot
 *  digraph Concepts {
 *      node [shape=record, fontname=Helvetica, fontsize=10];
 *      a [ label="IsCheckable" URL="@ref ConceptCheck"];
 *      b [ label="IsEqualityComparable" URL="@ref ConceptEquality"];
 *      c [ label="HasRandomInstance" URL="@ref ConceptRandomInstance"];
 *      d [ label="IsSerializable" URL="@ref ConceptSerialize"];
 *      e [ label="HasId" URL="@ref ConceptId"];
 *      f [ label="IsPrintable" URL="@ref ConceptPrint"];
 *      g [ label="IsStringConstructible" URL="@ref ConceptStringConstructible"];
 *      b -> a;
 *      c -> b;
 *      d -> b;
 *      e -> d;
 *      e -> c;
 *      f -> d;
 *      g -> f;
 *  }
 *  @enddot
 *@note This is only for the main classes done is our project. Use type_traits to test for other things
**/
namespace Concepts
{
/**
 *@class IsCheckable
 *@brief Allows for static and dynamic of the Check Concept
 *@pre One of the two following conditions must be followed :
 *- C does not have a check function
 *- C follows the Check Concept which is described @ref ConceptCheck "here".
 *@author Julien
**/
template<typename C>
struct IsCheckable
{

    static constexpr bool value=impl::HasCheckImpl<C, std::is_class<C>::value>::value;
    /**
     *@anchor ConceptCheck
    **/
    static void test(unsigned n = 1000);
};

/**
 *@class HasId
 *@brief Allows for static and dynamic testing of the Id Concept
 *@pre One of the two following conditions must be followed :
 *- C has none of the functions id, maxId, fromId
 *- C follows the IdConcept which is described @ref ConceptId "here".
 *@author Julien
**/
template<typename C>
struct HasId
{
    /**
     *@brief is true if C follows the Id concept (described @ref ConceptId "here").
     *@author Julien
     *@todo test for check and randomInstance
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::HasIdImpl<C, std::is_class<C>::value>::value;

    /**
     *@anchor ConceptId
     *@brief If HasId<C>::value = true, checks if C::id, C::maxId, C::fromId respect the dynamic part of the concept id concept. Otherwise does not do anything.
     *
     *@details Respecting the Id concept means :
     *- Having all the functions needed for all that follows (fails means ASSERT failed)
     *- From x = 0 to maxId -1 we have :
     *  -# fromId(x) never throws (or fails)
     *  -# We have check(fromId(x))
     *  -# id(fromId(x)) never throws (or fails) and is equal to x
     *- For each randomInstance :
     *  -# id(randomInstance) does not throw (or fail)
     *  -# id(randomInstance) is between 0 and maxId-1
     *- For each x >= maxId, fromId fails
     *
     *@throw never throws
     *@note this function is equivalent to a test.
     *@author Julien
    **/
    static void test(unsigned n = 1000);
};

template<typename C>
struct HasRandomInstance
{
    static constexpr bool value = impl::HasRandomInstanceImpl<C, std::is_class<C>::value>::value;
    /**
     *@anchor ConceptRandomInstance
    **/
    static void test(unsigned n = 1000);
};

template<typename C>
struct IsEqualityComparable
{

    static constexpr bool value = true;
    /**
     *@anchor ConceptEquality
    **/
    static void test(unsigned n = 1000);
};

template<typename C>
struct IsPrintable
{

    static constexpr bool value = true;
    /**
     *@anchor ConceptPrint
    **/
    static void test(unsigned n = 1000);
};

template<typename C>
struct IsSerializable
{

    static constexpr bool value = true;
    /**
     *@anchor ConceptSerialize
    **/
    static void test(unsigned n = 1000);
};

template<typename C>
struct IsStringConstructible
{

    static constexpr bool value = true;
    /**
     *@anchor ConceptStringConstructible
    **/
    static void test(unsigned n = 1000);
};
}


#endif // CONCEPTS_HPP_INCLUDED

