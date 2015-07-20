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

#include "Concepts.ipp"
/**
 *@file
 *@brief toto
**/
ENABLE_CALLABLE_TEST(id)
ENABLE_CALLABLE_TEST(maxId)
ENABLE_CALLABLE_TEST(fromId)
ENABLE_CALLABLE_TEST(randomInstance)


namespace impl {
template<typename C, bool b>
struct HasIdImpl;

template<typename C, bool b>
struct HasRandomInstanceImpl;
}

namespace impl {
template<typename C>
struct HasIdImpl<C, true>
{
    static constexpr bool value = IS_CALLABLE_MEMBER(id, int (C::*) ()) && IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && IS_CALLABLE_MEMBER(fromId, C (C::*) (int));
    static_assert(value || (!IS_CALLABLE_MEMBER(id, int (C::*) ()) && !IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && !IS_CALLABLE_MEMBER(fromId, C (C::*) (int))), "A class must either have all three functions id, maxId and fromId, or none");
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
};

template<typename C>
struct HasRandomInstanceImpl<C, false>
{
    static constexpr bool value = false;
};
}



#endif // CONCEPTS_IPP_H_INCLUDED
