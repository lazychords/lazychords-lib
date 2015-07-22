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
template<typename ...Args>\
struct CallableStatic_##F\
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
struct CallableStatic_##F<Ret (T::*)(Args...)>\
{\
\
    template<typename C>\
    static std::true_type test(decltype(C::F(std::declval<Args>()...)));\
\
    template<typename C> static std::false_type test(...);\
\
    static constexpr bool value = decltype(test<T>(std::declval<Ret>()))::value;\
};\
\
template<typename T, typename ...Args>\
struct CallableStatic_##F<void (T::*)(Args...)>\
{\
\
    template<typename C>\
    static std::true_type test(decltype(C::F(std::declval<Args>()...))*);\
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

namespace impl
{
template<typename ...Args>
struct Callable_opLeftShift
{
    static constexpr int value = -1;
};

template<typename T, typename Ret, typename ...Args>
struct Callable_opLeftShift<Ret (T::*)(Args...)>
{

    template<typename C>
    static std::true_type test(decltype(std::declval<C>().operator<<(std::declval<Args>()...)));

    template<typename C> static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(std::declval<Ret>()))::value;
};

template<typename T, typename ...Args>
struct Callable_opLeftShift<void (T::*)(Args...)>
{

    template<typename C>
    static std::true_type test(decltype(std::declval<C>().operator<<(std::declval<Args>()...))*);

    template<typename C> static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template<typename T, typename Ret, typename ...Args>
struct Callable_opLeftShift<Ret (T::*)(Args...) const>
{

    template<typename C>
    static std::true_type test(decltype(std::declval<C>().operator<<(std::declval<Args>()...)));

    template<typename C> static std::false_type test(...);

    static constexpr bool value = decltype(test<const T>(std::declval<Ret>()))::value;
};

template<typename T, typename ...Args>
struct Callable_opLeftShift<void (T::*)(Args...) const>
{

    template<typename C>
    static std::true_type test(decltype(std::declval<C>().operator<<(std::declval<Args>()...))*);

    template<typename C> static std::false_type test(...);

    static constexpr bool value = decltype(test<const T>(nullptr))::value;
};
}


namespace impl
{
template<typename ...Args>
struct Callable_opRightShift
{
    static constexpr int value = -1;
};

template<typename T, typename Ret, typename ...Args>
struct Callable_opRightShift<Ret (T::*)(Args...)>
{

    template<typename C>
    static std::true_type test(decltype(std::declval<C>().operator>>(std::declval<Args>()...)));

    template<typename C> static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(std::declval<Ret>()))::value;
};

template<typename T, typename ...Args>
struct Callable_opRightShift<void (T::*)(Args...)>
{

    template<typename C>
    static std::true_type test(decltype(std::declval<C>().operator>>(std::declval<Args>()...))*);

    template<typename C> static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template<typename T, typename Ret, typename ...Args>
struct Callable_opRightShift<Ret (T::*)(Args...) const>
{

    template<typename C>
    static std::true_type test(decltype(std::declval<C>().operator>>(std::declval<Args>()...)));

    template<typename C> static std::false_type test(...);

    static constexpr bool value = decltype(test<const T>(std::declval<Ret>()))::value;
};

template<typename T, typename ...Args>
struct Callable_opRightShift<void (T::*)(Args...) const>
{

    template<typename C>
    static std::true_type test(decltype(std::declval<C>().operator>>(std::declval<Args>()...))*);

    template<typename C> static std::false_type test(...);

    static constexpr bool value = decltype(test<const T>(nullptr))::value;
};
}
#include "Utilities.hpp"

GCC_IGNORE_WARNINGS
#include <boost/type_traits/has_operator.hpp>
GCC_PUT_BACK_WARNINGS

#define IS_CALLABLE_MEMBER(F, Type) impl::Callable_##F<Type>::value
#define IS_CALLABLE_MEMBER_LEFTSHIFT(Type) impl::Callable_opLeftShift<Type>::value
#define IS_CALLABLE_MEMBER_RIGHTSHIFT(Type) impl::Callable_opRightShift<Type>::value
#define IS_CALLABLE_MEMBER_STATIC(F, Type) impl::CallableStatic_##F<Type>::value

ENABLE_CALLABLE_TEST(id)
ENABLE_CALLABLE_TEST(maxId)
ENABLE_CALLABLE_TEST(fromId)
ENABLE_CALLABLE_TEST(randomInstance)
ENABLE_CALLABLE_TEST(check)
ENABLE_CALLABLE_TEST(save)
ENABLE_CALLABLE_TEST(load)
ENABLE_CALLABLE_TEST(fromStream)

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
    static constexpr bool value = IS_CALLABLE_MEMBER(id, int (C::*) ()) && IS_CALLABLE_MEMBER_STATIC(maxId, int (C::*) ()) && IS_CALLABLE_MEMBER_STATIC(fromId, C (C::*) (int));
    static_assert(value || (!IS_CALLABLE_MEMBER(id, int (C::*) ()) && !IS_CALLABLE_MEMBER_STATIC(maxId, int (C::*) ()) && !IS_CALLABLE_MEMBER_STATIC(fromId, C (C::*) (int))), "A class must either have all three functions id, maxId and fromId, or none.");
    static_assert(!value || (Concepts::IsSerializable<C>::value && Concepts::HasRandomInstance<C>::value), "Ideability implies RandomInstanciability and Serialization");
};

template<typename C>
struct HasIdImpl<C, false>
{
    static constexpr bool value = false;
};

template<typename C>
struct HasRandomInstanceImpl<C, true>
{
    static constexpr bool value = IS_CALLABLE_MEMBER_STATIC(randomInstance, C (C::*) ());
    static_assert(!value || (Concepts::IsCheckable<C>::value && Concepts::IsEqualityComparable<C>::value), "RandomInstanciability implies checkability and equality comparabality");

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
    static_assert((value && IS_CALLABLE_MEMBER_STATIC(load, C (C::*) (std::istream&))) || (!value && !IS_CALLABLE_MEMBER_STATIC(load, C (C::*) (std::istream&))), "The class must either have both load and save function, or none");
    static_assert(!value || (Concepts::IsCheckable<C>::value), "Serialization implies checkability");
};

template<typename C>
struct IsSerializableImpl<C, false>
{
    static constexpr bool value = false;
};

template<typename C>
constexpr bool f()
{
    return boost::has_left_shift<std::ostream&,const C&, std::ostream&>::value || IS_CALLABLE_MEMBER_LEFTSHIFT(std::ostream& (C::*) (std::ostream&));
}

template<typename C, bool b>
struct IsPrintableImpl;

template<typename C>
struct IsPrintableImpl<C, true>
{
    static constexpr bool value = boost::has_left_shift<std::ostream&,const C&, std::ostream&>::value || IS_CALLABLE_MEMBER_LEFTSHIFT(std::ostream& (C::*) (std::ostream&)const);
    static_assert(!value || !Concepts::IsCheckable<C>::value || Concepts::IsSerializable<C>::value, "Printable implies Serializable when it is one of our classes (=check defined)");
};

template<typename C>
struct IsPrintableImpl<C, false>
{
    static constexpr bool value = boost::has_left_shift<std::ostream&,const C&, std::ostream&>::value;
};

template<typename C, bool b>
struct IsReadableImpl;

template<typename C>
struct IsReadableImpl<C, true>
{
    static constexpr bool value = boost::has_right_shift<std::istream&, C&, std::istream&>::value || IS_CALLABLE_MEMBER_RIGHTSHIFT(std::istream& (C::*) (std::istream&));
    static_assert(!value || Concepts::IsPrintable<C>::value, "Readable implies Printable");
    static_assert(!value || !Concepts::IsCheckable<C>::value || IS_CALLABLE_MEMBER_STATIC(fromStream, C (C::*)(std::istream&)), "If a class of our project is readable, it has the function fromStream");

};

template<typename C>
struct IsReadableImpl<C, false>
{
    static constexpr bool value = boost::has_right_shift<std::istream&, C&, std::istream&>::value;
    static_assert(!value || Concepts::IsPrintable<C>::value, "Readable implies Printable");
};



}


#undef IS_CALLABLE_MEMBER
#undef IS_CALLABLE_MEMBER_LEFTSHIFT
#undef IS_CALLABLE_MEMBER_RIGHTSHIFT
#undef IS_CALLABLE_MEMBER_STATIC
#endif // CONCEPTS_IPP_H_INCLUDED
