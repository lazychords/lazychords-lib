
#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include <string>
#include <sstream>
#include <vector>

using Fraction =unsigned;

using Number = double;

class Time
{
public :
    enum Unit {H, MIN, S, MS, US};
    Time(float v = 0, Unit u = Time::S);
    Time& operator+=(const Time& o);
    Time operator+(const Time& o) const __attribute__((pure));
    Time& operator/=(float v);
    Time& operator*=(float v);
    Time operator/(float v) const __attribute__((pure));
    Time operator*(float v) const __attribute__((pure));
    float getNumber(Unit u = Time::S) const __attribute__((pure));
private :
    float number;

};

/**
 *@brief Puts a file into a string
 *@param str is the file name to read
 *@return the string in the file
**/
std::string getFile(const std::string& str);


template<typename T>
std::string toString(const T& a)
{
    std::ostringstream o;
    o<<a;
    return o.str();
}

template<typename T>
std::vector<std::vector<T> > createAllPossibilities(const std::vector<T>& v, unsigned l)
{
    if(l==0)
        return std::vector<std::vector<T> >(1, std::vector<T>());
    auto prec = createAllPossibilities(v, l-1);
    std::vector<std::vector<T> > res;
    for(unsigned i=0;i<v.size();i++)
    {
        for(unsigned j=0;j<prec.size();j++)
        {
            auto tmp(prec[j]);
            tmp.push_back(v[i]);
            res.push_back(tmp);
        }
    }
    return res;
}

#endif // UTILITIES_HPP_INCLUDED
