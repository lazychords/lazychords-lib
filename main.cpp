#include <iostream>
#include <fstream>
#include "src/Music.hpp"
#include <boost/filesystem.hpp>
#include "src/Concepts.hpp"

ENABLE_CALLABLE_TEST(id)
ENABLE_CALLABLE_TEST(toto)

void toto(double, int);

int main()
{
    std::cout<<"Pitch : "<<IS_CALLABLE_MEMBER(id, int (Pitch::*)(void))<<"\n";
    std::cout<<"const Pitch : "<<IS_CALLABLE_MEMBER(id, unsigned ( Pitch::*)(void) const)<<"\n";
    std::cout<<"Global toto : "<<IS_CALLABLE_GLOBAL(toto, unsigned, int, int)<<"\n";
    //Le truc qui bug chez moi mais qui ne devrait pas je crois
    //std::cout<<"const Note : "<<IS_CALLABLE_MEMBER(id, unsigned ( Note::*)(void) const)<<"\n";
    std::ofstream f("tmp.txt");
    std::cout.flush();
}
