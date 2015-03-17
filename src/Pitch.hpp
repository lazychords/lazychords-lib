/**
 * @file   Pitch.hpp
 * @author  <nicolas@archimede>
 * @date   Tue Mar 17 17:13:48 2015
 * 
 * @brief  This file defines the Pitch structure
 * 
 * 
 */
#ifndef PITCH_HPP_INCLUDED
#define PITCH_HPP_INCLUDED

#include <vector>
#include <iostream>

class Pitch;

class Pitch
{
private:
    unsigned halfTone;///<Number of half tones from C. Must be between 0 and 11;
public:
    bool check() const;
    void save(std::ostream& o) const;
    static Pitch load(std::istream& i);
    unsigned id() const;
    static Pitch fromId(unsigned hashValue);
    static Pitch randomInstance();
    static unsigned maxId();
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Pitch fromStream(std::istream& i);

    Pitch();
    Pitch(unsigned halfTones);
    Pitch(const Pitch&) = default;
    Pitch(Pitch&&) = default;
    Pitch& operator=(const Pitch&) = default;
    ~Pitch() = default;

    bool operator==(const Pitch&) const;
    bool operator!=(const Pitch&) const;

    Pitch& operator+=(int halfTones);
    Pitch& operator-=(int halfTones);
    Pitch operator+(int halfTones) const;
    Pitch operator-(int halfTones) const;

    static const Pitch C;
    static const Pitch Cs;
    static const Pitch Df;
    static const Pitch D;
    static const Pitch Ds;
    static const Pitch Ef;
    static const Pitch E;
    static const Pitch F;
    static const Pitch Fs;
    static const Pitch Gf;
    static const Pitch G;
    static const Pitch Gs;
    static const Pitch Af;
    static const Pitch A;
    static const Pitch As;
    static const Pitch Bf;
    static const Pitch B;
};


#endif
