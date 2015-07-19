//#define GENERIC_TEST(T) TEST_CASE("Generic Tests for #T", "[#T]"){generic_tests<T, "#T">::go();}

template<typename C, typename Test, typename = typename std::enable_if<Concepts::HasId<C>::value>::type >
void forAll(const Test& test, unsigned)
{
    for(unsigned i = 0; i < C::maxId(); i++)
    {
        C tmp = C::fromId(i);
        test(tmp);
    }
}

template<typename C, typename Test, typename = typename std::enable_if<!Concepts::HasId<C>::value>::type, typename =void >
void forAll(const Test& test, unsigned n)
{
    for(unsigned i = 0; i < n; i++)
    {
        C tmp = C::randomInstance();
        test(tmp);
    }
}

