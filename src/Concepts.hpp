
#ifndef CONCEPTS_HPP_INCLUDED
#define CONCEPTS_HPP_INCLUDED

#include "ConceptBase.hpp"

ENABLE_CALLABLE_TEST(id)
ENABLE_CALLABLE_TEST(maxId)
ENABLE_CALLABLE_TEST(fromId)

namespace impl {}
namespace Concepts
{

template<typename C>
struct HasId
{
    static constexpr bool value = IS_CALLABLE_MEMBER(id, int (C::*) ()) && IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && IS_CALLABLE_MEMBER(fromId, C (C::*) (int));
    static_assert(value || (!IS_CALLABLE_MEMBER(id, int (C::*) ()) && !IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && !IS_CALLABLE_MEMBER(fromId, C (C::*) (int))), "A class must either have all three functions id, maxId and fromId, or none");
};

}
#endif // CONCEPTS_HPP_INCLUDED

