
#ifndef CONCEPTS_HPP_INCLUDED
#define CONCEPTS_HPP_INCLUDED

#include "ConceptBase.hpp"

ENABLE_CALLABLE_TEST(id)
ENABLE_CALLABLE_TEST(maxId)
ENABLE_CALLABLE_TEST(fromId)
ENABLE_CALLABLE_TEST(randomInstance)

namespace impl {}

namespace impl {
template<typename C, bool b>
struct HasIdImpl;

template<typename C, bool b>
struct HasRandomInstanceImpl;
}

namespace Concepts
{
template<typename C>
struct HasId
{
    static constexpr bool value = impl::HasIdImpl<C, std::is_class<C>::value>::value;
};

template<typename C>
struct HasRandomInstance
{
    static constexpr bool value = impl::HasRandomInstanceImpl<C, std::is_class<C>::value>::value;
};
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
#endif // CONCEPTS_HPP_INCLUDED

