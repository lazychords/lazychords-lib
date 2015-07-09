#include "catch.hpp"
#include "../src/Concepts.hpp"
#include "Pitch.hpp"
#include "Figure.hpp"
/**Generic verifications :

Note :I will be implementing functions such as IsHashable<T>::value, IsComparable<T>::value, ...

For Hashable classes, all instances means for all ids, for the others it means for all random instances tested.
When needing to test strings, there will be a function testStringBegin() and a function testStingEnd() that will iterate on all strings to test.

Of course, do not forget that these are generic tests, you still have to write tests to try out all your functions

*Test for all classes*

- All instances are valid -> Try check on them
- Default constructor creates a valid class
- Operator >> never fails (nor throws)
- Function save never fails

*Test for comparable classes*
- For all instances, operator =, copy constructor, move constructor, ==, != are coherent

*Test for hashable classes*
-All hashable classes are comparable/loadable/<<
-If created from an invalid ID, fromID fails
-id(fromId(x))) = x
-fromId(id(c)) == c
-forall x <> y, fromId(x) != fromId(y)
-for all tries on randomInstance, id(randomInstance) is valid

*Tests for all loadable classes*

-load(save) = Identity (for comparable classes)
-if load fails/throws, the stream is not modified, if load succeeds, save(load) = Identity (load is not meant to be a human readable format)

*Tests for << classes*

- >>(<<) = Identity (for comparable classes)
-fromStream(in) = in >> DefaultConstructor or (throws an exception and >> fails)
- if << fails, the stream is not modified.

**/

//#define GENERIC_TEST(T) TEST_CASE("Generic Tests for #T", "[#T]"){generic_tests<T, "#T">::go();}

template<typename C, typename Test, typename = typename std::enable_if<Concepts::HasId<C>::value>::type >
void forAll(const Test& test, unsigned n = 1000)
{
    for(unsigned i = 0; i < C::maxId(); i++)
    {
        C tmp = C::fromId(i);
        test(tmp);
    }
}

template<typename C, typename Test, typename = typename std::enable_if<!Concepts::HasId<C>::value>::type, typename =void >
void forAll(const Test& test, unsigned n = 1000)
{
    for(unsigned i = 0; i < n; i++)
    {
        C tmp = C::randomInstance();
        test(tmp);
    }
}

///Just testing if test framework works
TEST_CASE("Testing test framework","[testFramework]")
{
    SECTION("success"){
        forAll<Pitch>([](const Pitch& p){REQUIRE(p.check());});
        forAll<Figure>([](const Figure& f){REQUIRE(f.check());});
    }
    SECTION("fail"){
	REQUIRE_FALSE(0 > 1);
    }
}
