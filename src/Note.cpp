#include "Note.hpp"

using namespace std;

Note::Note(const Fraction& duration, bool rest) :
    Pitch(),
    silence(rest),
    duration(duration)
{
    assert(check());
}

Note::Note(unsigned halfTones, const Fraction& duration) :
    Pitch(halfTones),
    silence(false),
    duration(duration)
{
    assert(check());
}

Note::Note(const Pitch& p, const Fraction& duration) :
    Pitch(p),
    silence(false),
    duration(duration)
{
    assert(check());
}


bool Note::check() const
{
    //duration==duration ensures that we do not have a NaN
    return Pitch::check() && duration>0 && duration<1e9 && duration==duration;
}

bool Note::operator==(const Note& other) const
{
    if(silence){
        return other.silence && duration==other.duration;
    }
    return Pitch::operator==(other) && duration==other.duration;
}

bool Note::operator!=(const Note& other) const
{
    return !(other == (*this));
}

Note& Note::operator+=(int halfTones)
{
    assert(check());
    Pitch::operator+=(halfTones);
    assert(check());
    return (*this);
}

Note Note::operator+(int halfTones) const
{
    assert(check());
    Note result(*this);
    result+=halfTones;
    assert(check());
    return result;
}


Note& Note::operator-=(int halfTones)
{
    assert(check());
    Pitch::operator-=(halfTones);
    assert(check());
    return (*this);
}

Note Note::operator-(int halfTones) const
{
    assert(check());
    Note result(*this);
    result-=halfTones;
    assert(check());
    return result;
}

bool Note::isRest() const
{
    return silence;
}

const Fraction& Note::getDuration() const
{
    return duration;
}

void Note::changeDuration(const Fraction& d)
{
    assert(check());
    duration = d;
    assert(check());
}
