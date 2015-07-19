#include "Pitch.hpp"
#include "Log.hpp"
#include "Random.hpp"
#include <random>
#include <chrono>
#include "Utilities.hpp"

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
    ASSERT(check());
}


template<class Archive>
void Pitch::serialize(Archive & ar, const unsigned int)
{
    ar & halfTone;
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
    ASSERT(check());
    halfTone = safe_mod(safe_cast<int>(halfTone)+added,12);
    ASSERT(check());
    return (*this);
}

Pitch& Pitch::operator-=(int sub)
{
    ASSERT(check());
    halfTone = safe_mod(safe_cast<int>(halfTone)-sub,12);
    ASSERT(check());
    return (*this);
}

Pitch Pitch::operator+(int add) const
{
    Pitch result(*this);
    result += add;
    ASSERT(result.check());
    return result;
}

Pitch Pitch::operator-(int sub) const
{
    Pitch result(*this);
    result -= sub;
    ASSERT(result.check());
    return result;
}

void Pitch::save(std::ostream& o) const
{
    boost::archive::text_oarchive oa(o);
    oa << (*this);
}

Pitch Pitch::load(std::istream& i)
{
    boost::archive::text_iarchive ia(i);
    Pitch result;
    ia >> result;
    return result;
}


Pitch Pitch::randomInstance()
{
      return Pitch(Random::uniform_int(0,11));
}
