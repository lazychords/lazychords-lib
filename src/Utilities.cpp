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

unsigned powUnsigned(unsigned base, unsigned power)
{
    unsigned x=1;
    for(unsigned i=0;i<power;i++)
    {
        ASSERT(std::numeric_limits<unsigned>::max()/base > x);
        x*=base;
    }
    return x;
}

bool isPowerUnsigned(unsigned base, unsigned x)
{
    ASSERT(base > 1);
    while(x >= base)
    {
        if(x%base !=0)
            return false;
        x/=base;
    }
    return x==1;
}

unsigned logUnsigned(unsigned base, unsigned x)
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

unsigned safeMod(int number, unsigned mod)
{
    ASSERT(mod != 0);
    if(number<0)
        return safeMod(number+safe_cast<int>(mod),mod);
    return safe_cast<unsigned>(number)%mod;
}


