/**
 * @file   Chord.hpp
 * @author  Julien and Nicolas
 * @date   Wed Mar 18 00:41:19 2015
 *
 * @brief  Describe the Chord structure
 *
 *
 */
#ifndef CHORD_HPP_INCLUDED
#define CHORD_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "Pitch.hpp"

class Chord
{
private :
    //??????
public :
    bool check() const;
    void save(std::ostream& o) const;
    static Chord load(std::istream& i);
    unsigned id() const;
    static Chord fromId(unsigned hashValue);
    static Chord randomInstance();
    static unsigned maxId();
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Chord fromStream(std::istream& i);

    Chord(/*????*/);
    Chord(const Chord&) = default;
    Chord(Chord&&) = default;
    Chord& operator=(const Chord&) = default;
    ~Chord() = default;

    bool operator==(const Chord&) const;
    bool operator!=(const Chord&) const;

    const std::vector<Pitch>& getNotes() const;
};

using ChordProgression = std::vector<Chord>;


#endif
