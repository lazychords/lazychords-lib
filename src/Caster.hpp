#ifndef CASTER_H
#define CASTER_H
#include<cassert>
#include<limits>
#include<iostream>
// base template
template <bool IsFromSigned, bool IsToSigned>
struct safe_static_cast_helper{
    template <typename TO, typename FROM>
    static inline TO cast(FROM from);
};

// template specialization for casting from an unsigned type into an unsigned type
template <>
struct safe_static_cast_helper<false, false>
{
    template <typename TO, typename FROM>
    static inline TO cast(FROM from)
    {
        assert(from >= std::numeric_limits<TO>::lowest());
        assert(from <= std::numeric_limits<TO>::max());

        return static_cast<TO>(from);
    }
};

// template specialization for casting from an unsigned type into a signed type
template <>
struct safe_static_cast_helper<false, true>
{
    template <typename TO, typename FROM>
    static inline TO cast(FROM from)
    {
        //assert(from >= std::numeric_limits<TO>::min());
        assert(from <= std::numeric_limits<TO>::max());

        return static_cast<TO>(from);
    }
};

// template specialization for casting from a signed type into an unsigned type
template <>
struct safe_static_cast_helper<true, false>
{
    template <typename TO>
    static inline TO cast(double from)
    {
        assert(from >= 0);

        assert(from >= std::numeric_limits<TO>::lowest());
        assert(from <= std::numeric_limits<TO>::max());

        return static_cast<TO>(from);
    }
    template <typename TO, typename FROM>
    static inline TO cast(FROM from)
    {
        // make sure the input is not negative
        assert(from >= 0);

        // assuring a positive input, we can safely cast it into its unsigned type and check the numeric limits
        typedef typename std::make_unsigned<FROM>::type UnsignedFrom;
        assert(static_cast<UnsignedFrom>(from) >= std::numeric_limits<TO>::lowest());
        assert(static_cast<UnsignedFrom>(from) <= std::numeric_limits<TO>::max());

        return static_cast<TO>(from);
    }
};

// template specialization for casting from a signed type into a signed type
template <>
struct safe_static_cast_helper<true, true>
{
    template <typename TO, typename FROM>
    static inline TO cast(FROM from)
    {
        assert(from >= std::numeric_limits<TO>::lowest());
        assert(from <= std::numeric_limits<TO>::max());

        return static_cast<TO>(from);
    }
};

/*template <typename TO, typename FROM>
inline TO safe_cast(FROM from)
{
#ifdef NDEBUG
    return TO(from);
#else
    // delegate the call to the proper helper class, depending on the signedness of both types
    return safe_static_cast_helper<std::numeric_limits<FROM>::is_signed, std::numeric_limits<TO>::is_signed>::template cast<TO>(from);
#endif
    //return static_cast<TO>(from);


}*/
#endif
