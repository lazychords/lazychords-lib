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


/*Idea for check function test
If(HasCheck<C>)
{
    Try to create instances with :
    -RandomInstance
    -fromId
    see if check(C) on those instances
}*/

/*Idea for EqualityComparable function test
If(IsEqualityComparable<C>)
{
    create as many (C, C', C'') possible with randomInstance (and maybe fromId)
    see if C==C, C'==C', C''==C''
    for all pairs (A,B) within C and C' and C'' (9 of them), see if A==B xor A!=B and A==B <=> B==A
    for all (A, B, C) picked in C, C', C'' (27 of them), check that A==B and B==C => A==C
}*/

/*Idea for RandomInstance function test. One could imagine storing previous results in a file, to have more accurate results when the class is serializable.
If(Randominstance<C>)
{
    create a map from C -> unsigned
    create a map from C*C -> unsigned
    call randomInstance many times and add 1 to the first map each time an element is generated. add 1 to the second map for each consecutive pair generated

    if(HasId<C>)
    {
       => You know the number of elements.
       So you can check that each element of the first map has near 1/maxId probability ad second map 1/(maxId *maxId)
    }
    else
    {
       => each element should have probability 0 (otherwise the class should have an Id function). So do the same, but assuming the probabilities should be 0
    }
}*/


