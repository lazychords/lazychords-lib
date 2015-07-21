/**\page page1 Generic verifications
\tableofcontents

This page describes all the generic verifications that can be made on our classes.
This verifications are made to ensure consistency of the api as well as correctness of the implementation.

In the following, "all instances" means all ids in the context of hashable classes, and for the others it means for all random instances tested (we generate a fixed number of them, by default 1000).

When needing to test strings, there will be a function testStringBegin() and a function testStingEnd() that will iterate on all strings to test.

Of course, do not forget that these are generic tests, you still have to write tests to try out all your functions

\section section1 Test for all classes

- All instances are valid -> Try check on them
- Default constructor creates a valid class
- Operator >> never fails (nor throws)
- Function save never fails

\section section2 Test for comparable classes
- For all instances, operator =, copy constructor, move constructor, ==, != are coherent

\section section3 Test for hashable classes
- All hashable classes are comparable/loadable/<<
- If created from an invalid ID, fromID fails
- id(fromId(x))) = x
- fromId(id(c)) == c
- forall x <> y, fromId(x) != fromId(y)
- for all tries on randomInstance, id(randomInstance) is valid

\section section4 Tests for all loadable classes

- load(save) = Identity (for comparable classes)
- if load fails/throws, the stream is not modified, if load succeeds, save(load) = Identity (load is not meant to be a human readable format)

\section section5 Tests for << classes

- >>(<<) = Identity (for comparable classes)
-fromStream(in) = in >> DefaultConstructor or (throws an exception and >> fails)
- if << fails, the stream is not modified.

**/


/**@brief This utility function applies the given test to all possible elements of a given class
 * @param test The test function
 * @param n If enumerations of all possible values of the class is impossible, this value controls the number of random instances that must be generated
 */
template<typename C, typename Test, typename = typename std::enable_if<Concepts::HasId<C>::value>::type >
void forAll(const Test& test, unsigned n = 1000);

template<typename C, typename Test, typename = typename std::enable_if<!Concepts::HasId<C>::value>::type,typename = void >
void forAll(const Test& test, unsigned n = 1000);

#define REQUIRE_PRECONDITION_FAIL(X) Log::setErrorsFatal(true); REQUIRE_THROWS_AS(X, AssertExcpt);
#define CHECK_PRECONDITION_FAIL(X) Log::setErrorsFatal(true); CHECK_THROWS_AS(X, AssertExcpt);
#include "genericTests.cxx"
