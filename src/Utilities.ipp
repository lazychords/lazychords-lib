#ifndef UTILITIES_IPP_H_INCLUDED
#define UTILITIES_IPP_H_INCLUDED

#include <type_traits>
#include "Concepts.hpp"
#include <chrono>

template<typename T>
std::string toString(const T& a)
{
    std::ostringstream o;
    o<<a;
    return o.str();
}

namespace boost { namespace serialization {

template <typename Archive, typename T>
void save(Archive& ar, ::boost::rational<T> const& r, unsigned /*version*/)
{
    T n = r.numerator(), d = r.denominator();
    ar & n;
    ar & d;
}

template <typename Archive, typename T>
void load(Archive& ar, ::boost::rational<T>& r, unsigned /*version*/)
{
    T n=1, d=1;
    ar & n;
    ar & d;
    r = ::boost::rational<T>(n, d);
}
}}

namespace log_impl{
//this is the implementation of the binary logarithm. Credit goes John Owens (http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog)
static constexpr unsigned int b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0,
                                 0xFF00FF00, 0xFFFF0000};

static constexpr unsigned binary_log_impl(unsigned x,unsigned r, int i)
{
    return i<=0 ?
        r :
        binary_log_impl(x, r | safe_cast<unsigned>(((x & b[i]) != 0) << i) , i-1);
}
}
constexpr unsigned binary_log(unsigned x)
{
    return (x&(x-1))==0 ? //test if power of two
        log_impl::binary_log_impl(x,((x & log_impl::b[0]) != 0),4) : //computation
        throw std::runtime_error(toString(x)+" is not a power of two");
}

namespace impl {
template<typename From, typename To>
struct SafeCast;
}


template<typename To, typename From>
To safe_cast(const From& arg)
{
    static_assert(std::is_convertible<From,To>::value, "From must be convertible to To");
    impl::SafeCast<From, To>::canConvert(arg);
    return To(arg);
}

namespace impl
{

template<typename From, typename To, typename Default, int F, bool Eq>
struct biggest_impl
{
    using type = To;
};

template<typename From, typename To, typename Default, int F>
struct biggest_impl<From, To, Default, F, true>
{
    using type = Default;
};

template<typename From, typename To, typename Default, bool Eq>
struct biggest_impl<From, To, Default, true, Eq>
{
    using type = From;
};


template<typename From, typename To, typename Default>
struct biggest
{
    using type = typename biggest_impl<From, To, Default,  sizeof(To) < sizeof(From), sizeof(From) == sizeof(To)>::type;
};

template<typename From, bool FromSigned, typename To, bool ToSigned>
struct IntegralCase;

template<typename From, typename To>
struct IntegralCase<From, true, To, true>
{
    static void canConvert(const From& arg)
    {
        using CompareTypeMin = typename biggest<From, To, From>::type;
        using CompareTypeMax = typename biggest<From, To, From>::type;
        CompareTypeMin mmin = static_cast<CompareTypeMin>(std::numeric_limits<To>::min());
        CompareTypeMax mmax = static_cast<CompareTypeMax>(std::numeric_limits<To>::min());
        ASSERT(mmin <= CompareTypeMin(arg));
        ASSERT(mmax >= CompareTypeMax(arg));
    }
};
template<typename From, typename To>
struct IntegralCase<From, false, To, false>
{
    static void canConvert(const From& arg)
    {
        using CompareTypeMin = typename biggest<From, To, From>::type;
        using CompareTypeMax = typename biggest<From, To, From>::type;
        CompareTypeMin mmin = static_cast<CompareTypeMin>(std::numeric_limits<To>::min());
        CompareTypeMax mmax = static_cast<CompareTypeMax>(std::numeric_limits<To>::min());
        ASSERT(mmin <= CompareTypeMin(arg));
        ASSERT(mmax >= CompareTypeMax(arg));
    }
};
template<typename From, typename To>
struct IntegralCase<From, false, To, true>
{
    static void canConvert(const From& arg)
    {
        using CompareTypeMin = typename biggest<From, To, To>::type;
        using CompareTypeMax = typename biggest<From, To, From>::type;
        CompareTypeMin mmin = static_cast<CompareTypeMin>(std::numeric_limits<To>::min());
        CompareTypeMax mmax = static_cast<CompareTypeMax>(std::numeric_limits<To>::min());
        ASSERT(mmin <= CompareTypeMin(arg));
        ASSERT(mmax >= CompareTypeMax(arg));
    }
};
template<typename From, typename To>
struct IntegralCase<From, true, To, false>
{
    static void canConvert(const From& arg)
    {
        using CompareTypeMin = typename biggest<From, To, From>::type;
        using CompareTypeMax = typename biggest<From, To, To>::type;
        CompareTypeMin mmin = static_cast<CompareTypeMin>(std::numeric_limits<To>::min());
        CompareTypeMax mmax = static_cast<CompareTypeMax>(std::numeric_limits<To>::min());
        ASSERT(mmin <= CompareTypeMin(arg));
        ASSERT(mmax >= CompareTypeMax(arg));
    }
};

template<typename From, typename To, bool areIntegrals, bool areFloatingPoints>
struct DispatchCase
{
    static void canConvert(const From&) {}
};

template<typename From, typename To>
struct DispatchCase<From, To, true, false>
{
    static void canConvert(const From& arg) {IntegralCase<From, std::is_signed<From>::value, To, std::is_signed<To>::value>::canConvert(arg);}
};

template<typename From, typename To>
struct SafeCast
{
    static void canConvert(const From& arg)
    {
        DispatchCase<From, To, std::is_integral<From>::value && std::is_integral<To>::value, std::is_floating_point<From>::value && std::is_floating_point<To>::value>::canConvert(arg);
    }
};


}

#endif // UTILITIES_IPP_H_INCLUDED
