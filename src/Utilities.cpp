#include "Utilities.hpp"
#include <fstream>
#include <stdexcept>

std::string getFile(const std::string& str)
{
    std::ifstream f(str);
    if(!f)
        throw std::runtime_error("Could not read file : " + str);
    std::ostringstream s;
    s<<f.rdbuf();
    return s.str();
}

unsigned pow(unsigned base, unsigned power)
{
    unsigned x=1;
    for(unsigned i=0;i<power;i++)
        x*=base;
    return x;
}

unsigned log(unsigned base, unsigned x)
{
    ASSERT(base > 1);
    unsigned p=0;
    while(x >= base)
    {
        ASSERT(x%base==0)
        x/=base;
        p++;
    }
    ASSERT(x==1);
    return p;
}
