
#ifndef CONCEPTS_HPP_INCLUDED
#define CONCEPTS_HPP_INCLUDED

#include "ConceptBase.hpp"

ENABLE_CALLABLE_TEST(id)
ENABLE_CALLABLE_TEST(maxId)
ENABLE_CALLABLE_TEST(fromId)

namespace impl {}
namespace Concepts
{

template<bool b>
struct StaticIf;

template<typename C>
struct HasId
{
    static constexpr bool value = IS_CALLABLE_MEMBER(id, int (C::*) ()) && IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && IS_CALLABLE_MEMBER(fromId, C (C::*) (int));
    static_assert(value || (!IS_CALLABLE_MEMBER(id, int (C::*) ()) && !IS_CALLABLE_MEMBER(maxId, int (C::*) ()) && !IS_CALLABLE_MEMBER(fromId, C (C::*) (int))), "A class must either have all three functions id, maxId and fromId, or none");
};

template<>
struct StaticIf<true>
{
    template<typename F1, typename F2>
    static F1 get(const F1& ft, const F2& ff) {return ft;}
};
template<>
struct StaticIf<false>
{
    template<typename F1, typename F2>
    static F2 get(const F1& ft, const F2& ff) {return ff;}
};

}
#endif // CONCEPTS_HPP_INCLUDED

