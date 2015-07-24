
#ifndef CONCEPTS_HPP_INCLUDED
#define CONCEPTS_HPP_INCLUDED


namespace Concepts
{
template<typename C>
struct IsCheckable;

template<typename C>
struct IsEqualityComparable;

template<typename C>
struct HasRandomInstance;

template<typename C>
struct IsSerializable;

template<typename C>
struct HasId;

template<typename C>
struct IsPrintable;

template<typename C>
struct IsReadable;

}

#include "Concepts.ipp"

namespace doc_impl
{
    /**
     *@brief This class is just meant for doxygen documentation, do not use it.
     *@author Julien
     *@todo Try to remove this class
    **/
    struct C
    {
        void check() const;
        bool operator==(const C& other) const;
        bool operator!=(const C& other) const;
        static void randomInstance();
        void save(std::ostream& o) const;
        static C load(std::istream& i);
        static unsigned maxId();
        unsigned id() const;
        static C fromId(unsigned id);
        std::ostream& operator<<(std::ostream& o) const;
        std::istream& operator>>(std::istream& i);
        static C fromStream(std::istream& i);
    };
}

/**
 *@brief This namespace is used for Concepts, that it to say for checking if types follow the desired behavior.
 *@details Here is the Concept implication graph
 *  @dot
 *  digraph Concepts {
 *      node [shape=record, fontname=Helvetica, fontsize=10];
 *      a [ label="Check Concept\n check" URL="@ref ConceptCheck"];
 *      b [ label="EqualityComparable Concept\n ==\n !=" URL="@ref ConceptEquality"];
 *      c [ label="RandomInstance Concept\n randomInstance" URL="@ref ConceptRandomInstance"];
 *      d [ label="Serialize Concept\n save\n load" URL="@ref ConceptSerialize"];
 *      e [ label="Id Concept\n maxId\n id\n fromId" URL="@ref ConceptId"];
 *      f [ label="Print Concept\n <<" URL="@ref ConceptPrint"];
 *      g [ label="Read Concept\n >>\n fromStream" URL="@ref ConceptRead"];
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
 *@todo Find a way to put the Concept name in bold in graph and add links in graph to functions
 *
 *@author Julien
**/
namespace Concepts
{

/**
 *@anchor CheckDescription
 *@fn void C::check() const
 *@brief Checks whether the classes invariants are met
 *@property for all x of type C, check(x)
 *@author Julien
**/

/**
 *@anchor ConceptCheck
 *@brief Allows for static and dynamic testing of the Check Concept
 *@pre One of the two following conditions must be followed :
 *- C does not have a check function
 *- C follows the Check Concept.
 *@details Respecting the Check concept means having :
 *  1. @ref CheckDescription "void C::check() const"
 *  .
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
     *@brief If IsCheckable<C>::value = true, checks if C::check respect the dynamic part of the Check concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing.
    **/
    static void test(unsigned n = 1000);
};

/**
 *@anchor EqualOperatorDescription
 *@fn void bool C::operator==(const C& other) const
 *@brief indicates whether to objects are semantically equal
 *@param other is the object to compare this with
 *@property if c==c' and c' == c'' then c==c''
 *@property for all c, c == c' xor c @ref DifferentOperatorDescription "!=" c'
 *@return if the two objects are equal
 *@author Julien
**/

/**
 *@anchor DifferentOperatorDescription
 *@fn void bool C::operator!=(const C& other) const
 *@brief indicates whether to objects are semantically not equal
 *@param other is the object to compare this with
 *@property for all c, c @ref EqualOperatorDescription "==" c' xor c != c'
 *@return if the two objects are not equal
 *@author Julien
**/

/**
 *@anchor ConceptEquality
 *@brief Allows for static and dynamic testing of the EqualityComparable Concept
 *@pre One of the two following conditions must be followed :
 *- C does not have operator== nor operator!= defined
 *- C follows the EqualityComparable Concept.
 *@details Respecting the EqualityComparable concept means having :
 *1. @ref EqualOperatorDescription "bool C::operator==(const C& other) const" as a member function or free function
 *2. @ref DifferentOperatorDescription "bool C::operator!=(const C& other) const" as a member function or free function
 *
 *@author Julien
**/
template<typename C>
struct IsEqualityComparable
{
    /**
     *@brief value is true if C has operator== and operator != defined
     *@warning For free functions, the value for a given class is calculated only the first time, so if == and != are not defined at this moment...
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::IsEqualityComparableImpl<C>::value;
    /**
     *@brief If IsEqualityComparable<C>::value = true, checks if C==C and C!=C respect the dynamic part of the EqualityComparable concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing and will thus be defined in genericTests.hxx
    **/
    static void test(unsigned n = 1000);
};

/**
 *@anchor RandomDescription
 *@fn void static C C::randomInstance()
 *@brief Creates a uniform random object of type C
 *@property for all x, y of type C, Pr(randomInstance() @ref EqualOperatorDescription "==" x) = Pr(randomInstance() @ref EqualOperatorDescription "==" y)
 *@author Julien
**/

/**
 *@anchor ConceptRandomInstance
 *@brief Allows for static and dynamic testing of the RandomInstance Concept
 *@pre One of the two following conditions must be followed :
 *- C does not have the function RandomInstance
 *- C follows the RandomInstance Concept.
 *@details Respecting the RandomInstance concept having
 *1. @ref RandomDescription "static void C::randomInstance()"
 *2. @ref ConceptCheck "Check concept"
 *3. @ref ConceptEquality "EqualityComparable concept"
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
    **/
    static void test(unsigned n = 1000);
};

/**
 *@anchor SaveDescription
 *@fn void C::save(std::ostream& o) const
 *@brief Saves the object in an possibly unreadable format into a stream
 *@param o is the stream to save into
 *@throw std::runtime_error("Unable to write into stream")
 *@post Nothing is deleted/changed in o, only a string is appended
 *@post Either o is unchanged and an exception was thrown or o.str() is appended with a string str uniquely representing : str(c) = str(c') <=> c==c'
 *@property @ref LoadDescription "load" and save are inverse functions.
 *@author Julien
**/

/**
 *@anchor LoadDescription
 *@fn static C C::load(std::istream& i)
 *@brief Creates an object from a stream in which an object was previously saved.
 *@param i is the stream to load from
 *@throw std::runtime_error("Unable to read from stream")
 *@throw std::runtime_error("Could not load object from stream")
 *@post Nothing is added/changed to i, only a string is consumed
 *@post Either i is unchanged and an exception was thrown or i.str() is consumed with a string str uniquely representing : str(c) = str(c') <=> c @ref EqualOperatorDescription "==" c'
 *@property load and @ref SaveDescription "save" are inverse functions.
 *@return The object created from the stream
 *@author Julien
**/

/**
 *@anchor ConceptSerialize
 *@brief Allows for static and dynamic testing of the Serialize Concept
 *@pre One of the two following conditions must be followed :
 *- C has neither load nor save function defined
 *- C follows the Serialize Concept.
 *@details Respecting the Print concept means having :
 *1. @ref SaveDescription "void C::save(std::ostream& ) const"
 *1. @ref LoadDescription "static C C::load(std::istream& )"
 *1. @ref ConceptCheck "Check concept"
 *1. @ref ConceptEquality "EqualityComparable concept"
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
     *@brief If IsSerializable<C>::value = true, checks if load and save respect the dynamic part of the Serialize concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing and will thus be defined in genericTests.hxx
    **/
    static void test(unsigned n = 1000);
};

/**
 *@anchor MaxIdDescription
 *@fn static unsigned C::maxId()
 *@brief Returns the number of different elements that the type C can represent
 *@return Returns the number of different elements that the type C can represent
 *@author Julien
**/

/**
 *@anchor IdDescription
 *@fn unsigned C::id() const
 *@brief Is a bijection from the set of elements of type C to [0, @ref MaxIdDescription "maxId" [
 *@return the id of the object
 *@post id is between [0, @ref MaxIdDescription "maxId"[
 *@post id uniquely identifies the object : id(c) = id(c') <=> c @ref EqualOperatorDescription "==" c'
 *@property @ref FromIdDescription "fromId" and Id are inverse functions
 *@author Julien
**/

/**
 *@anchor FromIdDescription
 *@fn static C C::fromId(unsigned id)
 *@brief Creates the object from an id
 *@param id is the id of the object
 *@pre id must be between [0, @ref MaxIdDescription "maxId" [
 *@return The object created
 *@property fromId and @ref IdDescription "Id" are inverse functions
 *@author Julien
**/

/**
 *@anchor ConceptId
 *@brief Allows for static and dynamic testing of the Id Concept
 *@pre One of the two following conditions must be followed :
 *- C has none of the functions id, maxId, fromId
 *- C follows the IdConcept.
 *@details Respecting the Id concept means having :
 *1. @ref MaxIdDescription "static unsigned C::maxId()"
 *1. @ref IdDescription "unsigned C::id() const"
 *1. @ref FromIdDescription "static C C::fromId(unsigned id)"
 *1. @ref ConceptSerialize "Serialize concept"
 *1. @ref ConceptRandomInstance "RandomInstance concept"
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
    **/
    static void test(unsigned n = 1000);
};

/**
 *@anchor LeftShiftDescription
 *@fn std::ostream& C::operator<<(std::ostream& o) const
 *@brief Saves the object in a human readable format into a stream. Should behave on C just like on any other class of the std.
 *@param o is the stream to save into
 *@return The stream
 *@throw Should respect the std::stream operators throwing behavior
 *@post If it succeeds, it appends the stream with a human readable string uniquely identifying the object. Otherwise, o is unchanged.
 *@note This function can also be non member
 *@property The result of this function should be loadable by @ref RightShiftDescription "operator>>" when the latter exists
 *@author Julien
**/

/**
 *@anchor ConceptPrint
 *@brief Allows for static and dynamic testing of the Print Concept
 *@pre One of the two following conditions must be followed :
 *- We can not write std::ostream& << const C&
 *- C follows the Print Concept.
 *@details Respecting the Print concept means having :
 *1. @ref LeftShiftDescription "std::ostream& C::operator<<(std::ostream& o) const" as a member function or free function
 *2. @ref ConceptEquality "EqualityComparable concept"
 *
 *@author Julien
**/
template<typename C>
struct IsPrintable
{
    /**
     *@brief value is true if we can call std::ostream& << C
     *@warning For free functions, the value for a given class is calculated only the first time, so if << is not defined at this moment...
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::IsPrintableImpl<C, std::is_class<C>::value>::value;
    /**
     *@brief If IsPrintable<C>::value = true, checks if ostream<<C respect the dynamic part of the Print concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing and will thus be defined in genericTests.hxx
    **/
    static void test(unsigned n = 1000);
};

/**
 *@anchor RightShiftDescription
 *@fn std::istream& C::operator>>(std::istream& i)
 *@brief Loads the object from a stream. Should behave on C just like on any other class of the std.
 *@param i is the stream to load from
 *@return The stream
 *@throw Should respect the std::stream operators throwing behavior
 *@post If it succeeds, it consumes the stream the end of the stream to construct the object, otherwise, i is unchanged.
 *@note This function can also be non member
 *@property  io @ref LeftShiftDescription "<<" c; io >> c' should always succeed when io is valid and give c @ref EqualOperatorDescription "==" c'
 *@author Julien
**/

/**
 *@anchor FromStreamDescription
 *@fn static C C::fromStream(std::istream& i)
 *@brief Loads the object from a stream.
 *@details Should behave exactly as
 *@code
 *C c;
 *if(!(i >> c))
 *  throw std::runtime_error("Unable to load object from stream")
 *@endcode
 *@todo finish documentation
 *@author Julien
**/

/**
 *@anchor ConceptRead
 *@brief Allows for static and dynamic testing of the Read Concept
 *@pre One of the two following conditions must be followed :
 *- We can not call std::istream& operator>> C&
 *- C follows the Read Concept.
 *@details Respecting the Read concept means having :
 *1. @ref RightShiftDescription "std::istream& C::operator<<(std::istream& i)" as a member function or free function
 *1. @ref FromStreamDescription "static C C::fromStream(std::istream& i)" if C is one o the classes of our project
 *1. @ref ConceptPrint "Print concept"
 *
 *@author Julien
**/
template<typename C>
struct IsReadable
{
    /**
     *@brief value is true if std::istream& >> C is callable
     *@warning For free functions, the value for a given class is calculated only the first time, so if >> is not defined at this moment...
     *@author Julien
     *@todo hide the value of value in the documentation
    **/
    static constexpr bool value = impl::IsReadableImpl<C, std::is_class<C>::value>::value;
     /**
     *@brief If IsReadable<C>::value = true, checks if operator<< respect the dynamic part of the Print concept. Otherwise does not do anything.
     *@param n is the number of instances on which we try the requirements
     *@note This function should only be used for testing and will thus be defined in genericTests.hxx
    **/
    static void test(unsigned n = 1000);
};
}


#endif // CONCEPTS_HPP_INCLUDED

