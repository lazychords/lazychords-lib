#ifndef CONCEPTBASE_HPP_INCLUDED
#define CONCEPTBASE_HPP_INCLUDED


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

#define IS_CALLABLE_MEMBER(F, Type) impl::Callable_##F<Type>::value

#endif // CONCEPTBASE_HPP_INCLUDED
