/**
 * @file   Note.hpp
 * @author  Julien and Nicolas
 * @date   Wed Mar 18 00:19:30 2015
 *
 * @brief  Describe the Note structure
 *
 *
 */
#ifndef NOTE_HPP_INCLUDED
#define NOTE_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "Utilities.hpp"
#include "Pitch.hpp"


class Note : public Pitch
{
private :
    bool silence;
    Fraction duration;
public :
    bool check() const;
    void save(std::ostream& o) const;
    static Note load(std::istream& i);
    static Note randomInstance();
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Note fromStream(std::istream& i);

    Note(const Fraction& duration = 1, bool rest = true);
    Note(unsigned halfTones, const Fraction& duration = 1);
    Note(const Pitch& p, const Fraction& duration);
    Note(const Note&) = default;
    Note(Note&&) = default;
    Note& operator=(const Note&) = default;
    ~Note() = default;

    bool operator==(const Note&) const;
    bool operator!=(const Note&) const;

    Note& operator+=(int halfTones);
    Note& operator-=(int halfTones);

    Note operator+(int halfTones) const;
    Note operator-(int halfTones) const;

    bool isRest() const;
    operator bool() const; //WTF ??
    const Fraction& getDuration() const;
    void changeDuration(const Fraction& d);

};

#endif
