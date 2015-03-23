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
float unitToMicro(Time::Unit u) __attribute__((const));
float unitToMicro(Time::Unit u)
{
    switch(u)
    {
    case Time::H :
        return 60 * unitToMicro(Time::MIN);
    case Time::MIN :
        return 60 * unitToMicro(Time::S);
    case Time::S :
        return 1000*unitToMicro(Time::MS);
    case Time::MS :
        return 1000*unitToMicro(Time::US);
    case Time::US :
        return 1;
    }
    throw std::runtime_error("Missing switch in unitToMicro");
}

Time::Time(float v, Unit u) : number(v*unitToMicro(u))
{
}


Time& Time::operator+=(const Time& o)
{
    number+=o.number;
    return *this;
}

Time Time::operator+(const Time& o) const
{
    Time tmp(*this);
    tmp+=o;
    return tmp;
}
float Time::getNumber(Unit u) const
{
    return number/unitToMicro(u);
}

Time& Time::operator/=(float v)
{
    number/=v;
    return *this;
}

Time& Time::operator*=(float v)
{
    number*=v;
    return *this;
}
Time Time::operator/(float v) const
{
    Time r(*this);
    r/=v;
    return r;
}

Time Time::operator*(float v) const
{
    Time r(*this);
    r*=v;
    return r;
}


