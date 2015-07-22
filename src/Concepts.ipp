#ifndef CONCEPTS_IPP_H_INCLUDED
#define CONCEPTS_IPP_H_INCLUDED

#define ENABLE_CALLABLE_TEST(F)\
namespace impl\
{\
template<typename ...Args>\
struct Callable_##F\
{\
    static constexpr int value = -1;\
};\
\
template<typename T, typename Ret, typename ...Args>\
struct Callable_##F<Ret (T::*)(Args...)>\
{\
\
    template<typename C>\
    static std::true_type test(decltype(std::declval<C>().F(std::declval<Args>()...)));\
\
    template<typename C> static std::false_type test(...);\
\
    static constexpr bool value = decltype(test<T>(std::declval<Ret>()))::value;\
};\
\
template<typename T, typename ...Args>\
struct Callable_##F<void (T::*)(Args...)>\
{\
\
    template<typename C>\
    static std::true_type test(decltype(std::declval<C>().F(std::declval<Args>()...))*);\
\
    template<typename C> static std::false_type test(...);\
\
    static constexpr bool value = decltype(test<T>(nullptr))::value;\
};\
\
template<typename T, typename Ret, typename ...Args>\
struct Callable_##F<Ret (T::*)(Args...) const>\
{\
\
    template<typename C>\
    static std::true_type test(decltype(std::declval<C>().F(std::declval<Args>()...)));\
\
    template<typename C> static std::false_type test(...);\
\
    static constexpr bool value = decltype(test<const T>(std::declval<Ret>()))::value;\
};\
\
template<typename T, typename ...Args>\
struct Callable_##F<void (T::*)(Args...) const>\
{\
\
    template<typename C>\
    static std::true_type test(decltype(std::declval<C>().F(std::declval<Args>()...))*);\
\
    template<typename C> static std::false_type test(...);\
\
    static constexpr bool value = decltype(test<const T>(nullptr))::value;\
};\
}

#include "Utilities.hpp"

GCC_IGNORE_WARNINGS
#include <boost/type_traits/has_operator.hpp>
GCC_PUT_BACK_WARNINGS



ENABLE_CALLABLE_TEST(id)
ENABLE_CALLABLE_TEST(maxId)
ENABLE_CALLABLE_TEST(fromId)
ENABLE_CALLABLE_TEST(randomInstance)
ENABLE_CALLABLE_TEST(check)
ENABLE_CALLABLE_TEST(save)
ENABLE_CALLABLE_TEST(load)


namespace impl {

template<typename C, bool b>
struct HasCheckImpl;


template<typename C>
struct HasCheckImpl<C, true>
{
    static constexpr bool value = IS_CALLABLE_MEMBER(check, void (C::*) () const);
};

template<typename C>
struct HasCheckImpl<C, false>
{
    static constexpr bool value = false;
};


template<typename C, bool b>
struct HasIdImpl;

template<typename C, bool b>
struct HasRandomInstanceImpl;

template<typename C>
struct HasIdImpl<C, true>
{
    static constexpr bool value = IS_CALLABLE_MEMBER(id, int (C::*) ()) && IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && IS_CALLABLE_MEMBER(fromId, C (C::*) (int));
    static_assert((value && Concepts::IsCheckable<C>::value && Concepts::HasRandomInstance<C>::value) || (!IS_CALLABLE_MEMBER(id, int (C::*) ()) && !IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && !IS_CALLABLE_MEMBER(fromId, C (C::*) (int))), "A class must either have all three functions id, maxId and fromId, or none. If it has id, maxId, and fromId, it must also have check and randomInstance.");
};

template<typename C>
struct HasIdImpl<C, false>
{
    static constexpr bool value = false;
};

template<typename C>
struct HasRandomInstanceImpl<C, true>
{
    static constexpr bool value = IS_CALLABLE_MEMBER(randomInstance, C (C::*) ());
    static_assert((value && Concepts::IsCheckable<C>::value) || ! value, "Having a randomInstance function implies having a check function.");

};

template<typename C>
struct HasRandomInstanceImpl<C, false>
{
    static constexpr bool value = false;
};

template<typename C>
struct IsEqualityComparableImpl
{
    static constexpr bool value = boost::has_equal_to<C, C, bool>::value;
    static_assert((value && boost::has_not_equal_to<C, C, bool>::value) || (!value && !boost::has_not_equal_to<C, C, bool>::value), "A type should either have both == and != defined or none of them");
};

template<typename C, bool b>
struct IsSerializableImpl;

template<typename C>
struct IsSerializableImpl<C, true>
{
    static constexpr bool value = IS_CALLABLE_MEMBER(save, void (C::*) (std::ostream&) const);
};

template<typename C>
struct IsSerializableImpl<C, false>
{
    static constexpr bool value = false;
};

}



#endif // CONCEPTS_IPP_H_INCLUDED
