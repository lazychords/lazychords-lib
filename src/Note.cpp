#include "Note.hpp"
#include "Utilities.hpp"
#include <random>
#include <chrono>



using namespace std;

Note::Note(const Fraction& d, bool rest) :
    Pitch(),
    silence(rest),
    duration(d)
{
    assert(check());
}

Note::Note(unsigned halfTones, const Fraction& d) :
    Pitch(halfTones),
    silence(false),
    duration(d)
{
    assert(check());
}

Note::Note(const Pitch& p, const Fraction& d) :
    Pitch(p),
    silence(false),
    duration(d)
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
    assert(result.check());
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
    assert(result.check());
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

template<class Archive>
void Note::serialize(Archive & ar, const unsigned int)
{
    // serialize base class information
    ar & boost::serialization::base_object<Pitch>(*this);
    ar & silence;
    ar & duration;
}

void Note::save(std::ostream& o) const
{
    boost::archive::text_oarchive oa(o);
    oa << (*this);
}

Note Note::load(std::istream& i)
{
    boost::archive::text_iarchive ia(i);
    Note result;
    ia >> result;
    return result;
}

Note Note::randomInstance()
{
      auto seed1 = std::chrono::system_clock::now().time_since_epoch().count();
      std::minstd_rand0 generator (seed1);
      auto num = (generator() % 4)+1;
      auto den = (generator() % (num*8)) + 1;
      Fraction l(static_cast<int>(num),static_cast<int>(den));
      if (generator() & 1){ //choose between rest and normal note
          return Note(l,true);
      }
      return Note(Pitch::randomInstance(),l);
}
