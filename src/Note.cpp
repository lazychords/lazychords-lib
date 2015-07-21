#include "Note.hpp"
#include "Utilities.hpp"
#include "Random.hpp"



using namespace std;

Note::Note(const UFraction& d, bool rest) :
    p(Pitch()),
    silence(rest),
    duration(d)
{
    check();
}

Note::Note(unsigned halfTones, const UFraction& d) :
    p(halfTones),
    silence(false),
    duration(d)
{
    check();
}

Note::Note(const Pitch& pitch, const UFraction& d) :
    p(pitch),
    silence(false),
    duration(d)
{
    check();
}


void Note::check() const
{
    p.check();
    ASSERT(duration.denominator()<=maxStep);
    ASSERT(isPowerUnsigned(2, duration.denominator()));
}

bool Note::operator==(const Note& other) const
{
    if(silence){
        return other.silence && duration==other.duration;
    }
    else
        return p==other.p && duration==other.duration;
}

bool Note::operator!=(const Note& other) const
{
    return !(other == (*this));
}

Note& Note::operator+=(int halfTones)
{
    check();
    p+=halfTones;
    check();
    return (*this);
}

Note Note::operator+(int halfTones) const
{
    check();
    Note result(*this);
    result+=halfTones;
    result.check();
    return result;
}


Note& Note::operator-=(int halfTones)
{
    check();
    p-=halfTones;
    check();
    return (*this);
}

Note Note::operator-(int halfTones) const
{
    check();
    Note result(*this);
    result-=halfTones;
    result.check();
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
    check();
    duration = d;
    check();
}

template<class Archive>
void Note::serialize(Archive & ar, const unsigned int)
{
    // serialize base class information
    ar & p;
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
    unsigned num = Random::uniform_int(1u, 4*maxStep);
    unsigned den = Random::uniform_int(1u, logUnsigned(2, maxStep));
    UFraction l(num,powUnsigned(2,den));
    if (Random::rand<bool>()==0){ //choose between rest and normal note
        return Note(l,true);
    }
    return Note(Pitch::randomInstance(),l);
}
