#include "Pitch.hpp"
#include "Log.hpp"

using namespace std;
const Pitch Pitch::C = Pitch(0);
const Pitch Pitch::Cs = Pitch(1);
const Pitch Pitch::Df = Pitch(1);
const Pitch Pitch::D = Pitch(2);
const Pitch Pitch::Ds = Pitch(3);
const Pitch Pitch::Ef = Pitch(3);
const Pitch Pitch::E = Pitch(4);
const Pitch Pitch::F = Pitch(5);
const Pitch Pitch::Fs = Pitch(6);
const Pitch Pitch::Gf = Pitch(6);
const Pitch Pitch::G = Pitch(7);
const Pitch Pitch::Gs = Pitch(8);
const Pitch Pitch::Af = Pitch(8);
const Pitch Pitch::A = Pitch(9);
const Pitch Pitch::As = Pitch(10);
const Pitch Pitch::Bf = Pitch(10);
const Pitch Pitch::B = Pitch(11);


Pitch::Pitch(unsigned halfTones) : halfTone(halfTones)
{
    assert(check());
}

bool Pitch::check() const
{
    if(halfTone>11){
        return false;
    }
    return true;
}


bool Pitch::operator==(const Pitch& other) const
{
    return halfTone==other.halfTone;
}

bool Pitch::operator!=(const Pitch& other) const
{
    return !(other == (*this));
}

Pitch& Pitch::operator+=(int added)
{
    assert(check());
    halfTone = (halfTone+added)%12;
    assert(check());
    return (*this);
}

Pitch& Pitch::operator-=(int sub)
{
    assert(check());
    halfTone = (halfTone-sub)%12;
    assert(check());
    return (*this);
}

Pitch Pitch::operator+(int add) const
{
    Pitch result(*this);
    result += add;
    assert(result->check());
    return result;
}

Pitch Pitch::operator-(int sub) const
{
    Pitch result(*this);
    result -= sub;
    assert(result->check());
    return result;
}
