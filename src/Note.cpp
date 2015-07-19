#include "Note.hpp"
#include "Utilities.hpp"
#include "Random.hpp"



using namespace std;

Note::Note(const UFraction& d, bool rest) :
    Pitch(),
    silence(rest),
    duration(d)
{
    ASSERT(check());
}

Note::Note(unsigned halfTones, const UFraction& d) :
    Pitch(halfTones),
    silence(false),
    duration(d)
{
    ASSERT(check());
}

Note::Note(const Pitch& p, const UFraction& d) :
    Pitch(p),
    silence(false),
    duration(d)
{
    ASSERT(check());
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
    ASSERT(check());
    Pitch::operator+=(halfTones);
    ASSERT(check());
    return (*this);
}

Note Note::operator+(int halfTones) const
{
    ASSERT(check());
    Note result(*this);
    result+=halfTones;
    ASSERT(result.check());
    return result;
}


Note& Note::operator-=(int halfTones)
{
    ASSERT(check());
    Pitch::operator-=(halfTones);
    ASSERT(check());
    return (*this);
}

Note Note::operator-(int halfTones) const
{
    ASSERT(check());
    Note result(*this);
    result-=halfTones;
    ASSERT(result.check());
    return result;
}

bool Note::isRest() const
{
    return silence;
}

const UFraction& Note::getDuration() const
{
    return duration;
}

void Note::changeDuration(const UFraction& d)
{
    ASSERT(check());
    duration = d;
    ASSERT(check());
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
    //picking random integers > 1
    unsigned num = Random::uniform_int(1);
    unsigned den = Random::uniform_int(1);
    UFraction l(num,den);
    if (Random::random_bool()==0){ //choose between rest and normal note
        return Note(l,true);
    }
    return Note(Pitch::randomInstance(),l);
}
