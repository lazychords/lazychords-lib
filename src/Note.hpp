/**
 * @file   Note.hpp
 * @author  <nicolas@archimede>
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
#include <boost/rational.hpp>
#include "Pitch.hpp"

using Fraction = boost::rational<int>;

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
    Note(unsigned halfTones);
    Note(unsigned halfTones, const Fraction& duration);
    Note(const Pitch& p, const Fraction& duration);
    Note(const Note&) = default;
    Note(Note&&) = default;
    Note& operator=(const Note&) = default;
    ~Note() = default;

    bool operator==(const Note&) const;
    bool operator!=(const Note&) const;

    Note operator+(int halfTones) const;
    Note operator-(int halfTones) const;

    bool isrest() const;
    operator bool() const;
    const Fraction& getDuration() const;
    void changeDuration(const Fraction& d);

};

#endif
