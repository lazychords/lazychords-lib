#ifndef CONCEPTS_HPP_INCLUDED
#define CONCEPTS_HPP_INCLUDED


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
}\
std::false_type F(...);

#define IS_CALLABLE_MEMBER(F, Type) impl::Callable_##F<Type>::value

namespace impl
{
template<typename A, typename B>
struct Can_Convert;

template<typename B>
struct Can_Convert<std::false_type*, B*>
{
    static constexpr bool value = false;
};

template<>
struct Can_Convert<void*, void*>
{
    static constexpr bool value = true;
};

template<typename B>
struct Can_Convert<void*, B*>
{
    static constexpr bool value = false;
};

template<typename A>
struct Can_Convert<A*, void*>
{
    static constexpr bool value = true;
};

template<typename A, typename B>
struct Can_Convert<A*, B*>
{
    static constexpr bool value = std::is_convertible<A,B>::value;
};
}
#define PP_NARG( ...) PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(_1,_2,_3,_4,_5,_6,_7,_8,_9, _10, _11, _12, N, ...) N
#define PP_RSEQ_N() 12,11,10,9,8,7,6,5,4,3,2,1,0

#define IS_CALLABLE_GLOBAL(F, RET, ...) impl::Can_Convert<decltype(F(CONSTRUCT_TMP(PP_NARG(__VA_ARGS__), __VA_ARGS__)))*, RET*>::value


#define CONSTRUCT_TMP(N, ...) CONSTRUCT(N, __VA_ARGS__)
#define CONSTRUCT(N, ...) CONSTRUCT_ ## N ## _(__VA_ARGS__)


#define CONSTRUCT_1_(PARAM) std::declval<PARAM>()
#define CONSTRUCT_2_(PARAM1, PARAM2) std::declval<PARAM1>(), std::declval<PARAM2>()
#define CONSTRUCT_3_(PARAM1, PARAM2, PARAM3) std::declval<PARAM1>(), std::declval<PARAM2>(), std::declval<PARAM3>()







#endif // CONCEPTS_HPP_INCLUDED

