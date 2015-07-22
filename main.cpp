#include <iostream>
#include <fstream>
#include "src/Music.hpp"
#include "src/Concepts.hpp"
#include "src/Random.hpp"
#include "src/Note.hpp"

class mTest
{

};
int main()
{
    std::cout<<Concepts::IsSerializable<Pitch>::value<<" "<<Concepts::IsSerializable<mTest>::value<<"\n";
    std::cout<<"hello world\n";
    std::cout<<Random::uniform_int<uint64_t>()<<std::endl;
    std::cout<<Random::uniform_int<uint64_t>()<<std::endl;
    std::cout<<Random::uniform_int<uint64_t>()<<std::endl;
    /*std::cout<<Random::uniform_real<double>()<<std::endl;
    int truc = Random::rand<int>();
    std::cout<<truc<<std::endl;
    Note bidule = Random::rand<Note>();*/
}
