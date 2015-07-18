#include "Chord.hpp"

unsigned Chord::id() const
{
    return base.id()*5*4 + static_cast<unsigned>(fifth)*4 + static_cast<unsigned>(seventh);
}

Chord Chord::fromId(unsigned hashValue)
{
    if (hashValue >= maxId())
        throw std::runtime_error("Invalid ID for Chord::fromId()");
    Chord::Type7 seventh = static_cast<Type7>(hashValue%4);
    Chord::Type5 fifth = static_cast<Type5>((hashValue/4)%5);
    Pitch base(hashValue/(4*5)); 
    return Chord(base,fifth,seventh);
}

Chord Chord::randomInstance()
{
    return fromId(rand()%maxId());
}

unsigned Chord::maxId()
{
    return 12*5*4;
}

Chord::Chord(const Pitch& p, const Type5& t5) : base(p), fifth(t5), seventh(Chord::NONE)
{
}

Chord::Chord(const Pitch& p, const Type5& t5, const Type7& t7) : base(p), fifth(t5), seventh(t7)
{
}

bool Chord::operator==(const Chord& other) const
{
    return base==other.base && fifth==other.fifth && seventh==other.seventh;
}

bool Chord::operator!=(const Chord& other) const
{
    return !(*this==other);
}

std::vector<Pitch> Chord::getNotes() const
{
    std::vector<Pitch> noteList;
    noteList.push_back(base);
    switch (fifth)
    {
        case MAJ5:
            noteList.push_back(base+4);
            noteList.push_back(base+7);
            break;
        case MIN5:
            noteList.push_back(base+3);
            noteList.push_back(base+7);
            break;
        case AUG5:
            noteList.push_back(base+4);
            noteList.push_back(base+8);
            break;
        case DIM5:
            noteList.push_back(base+3);
            noteList.push_back(base+6);
            break;
        case SUS45:
            noteList.push_back(base+5);
            noteList.push_back(base+7);
            break;
        default:
            noteList.push_back(base+4);
            noteList.push_back(base+7);
    }
    switch (seventh)
    {
        case MAJ7:
            noteList.push_back(base+11);
            break;
        case MIN7:
            noteList.push_back(base+10);
            break;
        case DIM7:
            noteList.push_back(base+9);
            break;
        default:
            break;
    }
    return noteList;
}
