#include <iostream>
#include <fstream>
#include "src/Music.hpp"
#include "src/Concepts.hpp"
#include "src/Random.hpp"
#include "src/Note.hpp"


int main()
{
    std::cout<<"hello world\n";
    std::cout<<Random::uniform_int<uint64_t>()<<std::endl;
    std::cout<<Random::uniform_real<double>()<<std::endl;
    int truc = Random::rand<int>();
    std::cout<<truc<<std::endl;
    Note bidule = Random::rand<Note>();
}
