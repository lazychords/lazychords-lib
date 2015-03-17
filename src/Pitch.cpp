#include "Pitch.hpp"

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


bool Pitch::check() const
{
    if(halfTone>11){
        return false;
    }
    return true;
}

