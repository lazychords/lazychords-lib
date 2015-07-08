#include <iostream>
#include <fstream>
#include "src/Music.hpp"
#include <boost/filesystem.hpp>
#include "src/Concepts.hpp"

ENABLE_CALLABLE_TEST(id)

struct testTT
{
    static testTT randomInstance(int i);
};

void toto(double, int);

template<typename D>
bool CheckInstances(int n)
{
    auto f = DoIf<IS_CALLABLE_MEMBER(id, int (D::*) ())>::get([](){for(int i=0;i<Pitch::maxId();i++); return true;}, [n](){for(int i=0;i<n;i++) auto tmp = Pitch::randomInstance(); return false;});
    return f();
}
int main()
{
    std::cout<<CheckInstances<Pitch>(2);
    std::cout<<CheckInstances<testTT>(2);
}
