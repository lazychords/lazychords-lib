
#ifndef CONCEPTS_HPP_INCLUDED
#define CONCEPTS_HPP_INCLUDED


namespace Concepts
{
template<typename C>
struct IsCheckable;

template<typename C>
struct HasId;

template<typename C>
struct HasRandomInstance;

template<typename C>
struct IsEqualityComparable;

template<typename C>
struct IsSerializable;

template<typename C>
struct IsPrintable;

template<typename C>
struct IsReadable;

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
 *      g [ label="IsReadable" URL="@ref ConceptReadable"];
 *      c -> a;
 *      c -> b;
 *      d -> a;
 *      d -> b;
 *      e -> d;
 *      e -> c;
 *      f -> b;
 *      g -> f;
 *  }
 *  @enddot
 *@note Dotted lines is for classes that are from our project only.
**/
namespace Concepts
{
/**
 *@class IsCheckable
 *@brief Allows for static and dynamic testing of the Check Concept
 *@pre One of the two following conditions must be followed :
 *- C does not have a check function
 *- C follows the Check Concept.
 *@details Respecting the Check concept means :
 *1. Having the static requirements
 *  - void C::check() const exists
 *2. Having the dynamic requirements
 *  - for all x, check(x) does not fail
 *
 *@author Julien
**/
template<typename C>
struct IsCheckable
{
    /**
     *@brief is true if C has a check function
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value=impl::HasCheckImpl<C, std::is_class<C>::value>::value;
    /**
     *@anchor ConceptCheck
     *@brief If IsCheckable<C>::value = true, checks if C::check respect the dynamic part of the Check concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing.
     *@author Julien
    **/
    static void test(unsigned n = 1000);
};

/**
 *@class HasId
 *@brief Allows for static and dynamic testing of the Id Concept
 *@pre One of the two following conditions must be followed :
 *- C has none of the functions id, maxId, fromId
 *- C follows the IdConcept.
 *@details Respecting the Id concept means :
 *1. Having the static requirements
 *  - unsigned C::id() const exists
 *  - static unsigned C::maxId() exists
 *  - static C C::fromId(unsigned) exists
 *  - Having the static requirements of the @ref ConceptSerialize "Serialize concept"
 *  - Having the static requirements of the @ref ConceptRandomInstance "RandomInstance concept"
 *2. Having the dynamic requirements
 *  - Having the dynamic requirements of @ref ConceptSerialize "Serialize concept"
 *  - Having the dynamic requirements of @ref ConceptRandomInstance "RandomInstance concept"
 *  - for all x, fromId(id(x)) = x
 *  - for all 0 <= i < maxId, id(fromId(i)) = i
 *  - for i >=maxId, fromId(i) fails
 *
 *@author Julien
**/
template<typename C>
struct HasId
{
    /**
     *@brief value is true if C has the functions id, maxId, fromId.
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::HasIdImpl<C, std::is_class<C>::value>::value;

    /**
     *@brief If HasId<C>::value = true, checks if C::id, C::maxId, C::fromId respect the dynamic part of the Id concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing.
     *@author Julien
    **/
    static void test(unsigned n = 1000);
};

/**
 *@class HasRandomInstance
 *@anchor ConceptRandomInstance
 *@brief Allows for static and dynamic testing of the RandomInstance Concept
 *@pre One of the two following conditions must be followed :
 *- C does not have the function RandomInstance
 *- C follows the RandomInstance Concept.
 *@details Respecting the RandomInstance concept means :
 *1. Having the static requirements
 *  - static C C::randomInstance() exists
 *  - Having the static requirements of the @ref ConceptCheck "Check concept"
 *  - Having the static requirements of the @ref ConceptEquality "EqualityComparable concept"
 *2. Having the dynamic requirements
 *  - Having the dynamic requirements of @ref ConceptCheck "Check concept"
 *  - Having the dynamic requirements of @ref ConceptEquality "EqualityComparable concept"
 *  - |Pr(randomInstance() == randomInstance()) - 1/card(C)| < eps
 *
 *@author Julien
**/
template<typename C>
struct HasRandomInstance
{
    /**
     *@brief value is true if C has the function randomInstance
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::HasRandomInstanceImpl<C, std::is_class<C>::value>::value;
    /**
     *@brief If HasRandomInstance<C>::value = true, checks if C::randomInstance respect the dynamic part of the RandomInstance concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing.
     *@author Julien
    **/
    static void test(unsigned n = 1000);
};

/**
 *@class IsEqualityComparable
 *@anchor ConceptEquality
 *@brief Allows for static and dynamic testing of the EqualityComparable Concept
 *@pre One of the two following conditions must be followed :
 *- C does not have operator== nor operator!= defined
 *- C follows the EqualityComparable Concept.
 *@details Respecting the EqualityComparable concept means :
 *1. Having the static requirements
 *  - bool C::operator==(const C&) const exists
 *  - bool C::operator!=(const C&) const exists
 *2. Having the dynamic requirements
 *  - For all instances, we either have C==C or C!=C
 *  - If C==C' and C'== C'', then C==C''
 *  - If C==C' and C' != C'' then C != C''
 *
 *@author Julien
**/
template<typename C>
struct IsEqualityComparable
{
    /**
     *@brief value is true if C has operator== and operator != defined
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::IsEqualityComparableImpl<C>::value;
    /**
     *@brief If IsEqualityComparable<C>::value = true, checks if C==C and C!=C respect the dynamic part of the EqualityComparable concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing and will thus be defined in genericTests.hxx
     *@author Julien
    **/
    static void test(unsigned n = 1000);
};

/**
 *@class IsPrintable
 *@anchor ConceptPrint
 *@brief Allows for static and dynamic testing of the Print Concept
 *@pre One of the two following conditions must be followed :
 *- We can not write std::ostream& << const C&
 *- C follows the Print Concept.
 *@details Respecting the Print concept means :
 *1. Having the static requirements
 *  - Either std::ostream& operator<<(std::ostream&, const C&) exists or std::ostream& C::operator<<(std::ostream&) const exists
 *  - Having the static requirements of the @ref ConceptEquality "EqualityComparable concept"
 *2. Having the dynamic requirements
 *  - Having the dynamic requirements of the @ref ConceptEquality "EqualityComparable concept"
 *  - For all instances and valid ostream, std::ostream& << C works
 *  - For all instances and valid ostream, std::ostream& << C should only append to the stream and the string appended should be a unique identifier of C
 *
 *@author Julien
**/
template<typename C>
struct IsPrintable
{
    /**
     *@brief value is true if we can call std::ostream& << C
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::IsPrintableImpl<C, std::is_class<C>::value>::value;
    /**
     *@brief If IsPrintable<C>::value = true, checks if ostream<<C respect the dynamic part of the Print concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing and will thus be defined in genericTests.hxx
     *@author Julien
    **/
    static void test(unsigned n = 1000);
};

/**
 *@class IsSerializable
 *@anchor ConceptSerialize
 *@brief Allows for static and dynamic testing of the Serialize Concept
 *@pre One of the two following conditions must be followed :
 *- C has neither load nor save function defined
 *- C follows the Serialize Concept.
 *@details Respecting the Print concept means :
 *1. Having the static requirements
 *  - void save(std::ostream& ) const exists
 *  - static C load(std::istream&) exists
 *  - Having the static requirements of the @ref ConceptCheck "Check concept"
 *  - Having the static requirements of the @ref ConceptEquality "EqualityComparable concept"
 *2. Having the dynamic requirements
 *  - Having the dynamic requirements of the @ref ConceptCheck "Check concept"
 *  - Having the dynamic requirements of the @ref ConceptEquality "EqualityComparable concept"
 *  - for all x, C::load(x.save(std::stringstream&)) == x
 *  - for all x, (x.save(C::load(std::stringstream& s))).str() == s.str() or fails on the load
 *
 *@author Julien
**/
template<typename C>
struct IsSerializable
{
    /**
     *@brief value is true if C has save and load functions
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::IsSerializableImpl<C, std::is_class<C>::value>::value;
     /**
     *@brief If IsSerializable<C>::value = true, checks if load and save respect the dynamic part of the Print concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing and will thus be defined in genericTests.hxx
     *@author Julien
    **/
    static void test(unsigned n = 1000);
};

/**
 *@class IsReadable
 *@anchor ConceptRead
 *@brief Allows for static and dynamic testing of the Read Concept
 *@pre One of the two following conditions must be followed :
 *- We can not call std::istream& operator>> C&
 *- C follows the Read Concept.
 *@details Respecting the Read concept means :
 *1. Having the static requirements
 *  - We can call std::istream& operator>> C&
 *  - Having the static requirements of the @ref ConceptPrint "Print concept"
 *2. Having the dynamic requirements
 *  - Having the dynamic requirements of the @ref ConceptPrint "Print concept"
 *  - for all x, valid stringstream s, s<<x; s>>y gives x==y
 *  - for all x, valid stringstream s where s.exceptions is not set, s>>x; s'<< x gives s.str() = s'.str() or s>>x returned false and s and x are unchanged in the process.
 *
 *@author Julien
**/
template<typename C>
struct IsReadable
{
    /**
     *@brief value is true if std::istream& >> C is callable
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::IsReadableImpl<C, std::is_class<C>::value>::value;
     /**
     *@brief If IsReadable<C>::value = true, checks if operator<< respect the dynamic part of the Print concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing and will thus be defined in genericTests.hxx
     *@author Julien
    **/
    static void test(unsigned n = 1000);
};
}


#endif // CONCEPTS_HPP_INCLUDED

