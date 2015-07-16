#ifndef UTILITIES_IPP_H_INCLUDED
#define UTILITIES_IPP_H_INCLUDED

template<typename T>
std::string toString(const T& a)
{
    std::ostringstream o;
    o<<a;
    return o.str();
}

namespace boost { namespace serailization {

template <typename Archive, typename T>
void save(Archive& ar, ::boost::rational<T> const& r, unsigned /*version*/)
{
    int n = r.numerator(), d = r.denominator();
    ar & n;
    ar & d;
}

template <typename Archive, typename T>
void load(Archive& ar, ::boost::rational<T>& r, unsigned version)
{
    int n, d;
    ar & n;
    ar & d;
    r = ::boost::rational<T>(n, d);
}
}}

#endif // UTILITIES_IPP_H_INCLUDED
