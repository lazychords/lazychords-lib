#include <iostream>
#include <fstream>
#include "src/Music.hpp"
#include <boost/filesystem.hpp>
#include "src/Concepts.hpp"

using namespace Concepts;
struct testTT
{
    static testTT randomInstance(int i);
};

struct error
{
    static int maxId();
};

void toto(double, int);

template<typename D>
bool CheckInstances(int n)
{
    auto f = Concepts::StaticIf<HasId<D>::value>::get([](){for(int i=0;i<Pitch::maxId();i++); return true;}, [n](){for(int i=0;i<n;i++) auto tmp = Pitch::randomInstance(); return false;});
    return f();
}
int main()
{
    std::cout<<Concepts::HasId<Pitch>::value;
    std::cout<<Concepts::HasId<Chord>::value;
    std::cout<<Concepts::HasId<testTT>::value;
    //std::cout<<Concepts::HasId<error>::value;
}
