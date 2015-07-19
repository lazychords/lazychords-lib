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
    {
        ASSERT(std::numeric_limits<unsigned>::max()/base > x);
        x*=base;
    }
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

unsigned safe_mod(int number, unsigned mod)
{
    ASSERT(mod != 0);
    if(number<0)
        return safe_mod(number+mod,mod);
    return safe_cast<unsigned>(number%mod);
}
