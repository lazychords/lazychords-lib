#include <iostream>
#include <fstream>
#include "src/Music.hpp"
#include "src/Concepts.hpp"
#include "src/Random.hpp"
#include "src/Note.hpp"

//std::ostream& operator<<(std::ostream&, const Pitch&);



class mTest
{

};
int main()
{
    std::cout<<Concepts::IsSerializable<Pitch>::value<<" "<<Concepts::IsSerializable<mTest>::value<<"\n";
    std::cout<<Concepts::IsPrintable<int>::value<<"\n";
    std::cout<<Concepts::IsPrintable<std::vector<int> >::value<<"\n";
    std::cout<<Concepts::IsPrintable<mTest>::value<<"\n";
    std::cout<<Concepts::IsPrintable<Pitch>::value<<"\n";

    std::cout<<Concepts::IsReadable<int>::value<<"\n";
    std::cout<<Concepts::IsReadable<std::vector<int> >::value<<"\n";
    std::cout<<Concepts::IsReadable<mTest>::value<<"\n";
    std::cout<<Concepts::IsReadable<Pitch>::value<<"\n";
    std::cout<<"hello world\n";
    std::cout<<Random::uniform_int<uint64_t>()<<std::endl;
    std::cout<<Random::uniform_int<uint64_t>()<<std::endl;
    std::cout<<Random::uniform_int<uint64_t>()<<std::endl;
    /*std::cout<<Random::uniform_real<double>()<<std::endl;
    int truc = Random::rand<int>();
    std::cout<<truc<<std::endl;
    Note bidule = Random::rand<Note>();*/
}
