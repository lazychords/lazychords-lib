
#ifndef MUSIC_HPP_INCLUDED
#define MUSIC_HPP_INCLUDED

/**
 *@file
 *@brief File that includes the whole BaseMusic Module
**/

/**
 *@fn template<typename T> bool T::check() const
 *@pre This function should exist for any T in the public interface of the BaseMusic Module
 *@throw This function never throws
 *@return true if the invariants are respected
 *@note this function is meant to be used in the following way : ASSERT(check())
**/

#include "Chord.hpp"
#include "Figure.hpp"
#include "Key.hpp"
#include "Melody.hpp"
#include "Note.hpp"
#include "Pitch.hpp"
#include "Signature.hpp"


#endif // MUSIC_HPP_INCLUDED
