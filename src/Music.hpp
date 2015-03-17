/**
 * @file   Music.hpp
 * @date   Mon Oct  6 00:26:17 2014
 *
 * @brief  This file defines all the data structures related to music objects
 *
 *
 */

#ifndef MUSIC_HPP_INCLUDED
#define MUSIC_HPP_INCLUDED

#define BOOST

#include <map>
#include <vector>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>
#ifdef BOOST
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wsuggest-attribute=const"
#pragma GCC diagnostic ignored "-Wsuggest-attribute=pure"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/rational.hpp>
#pragma GCC diagnostic pop
#endif

/** @enum NoteName
 *  @brief Describe the name of the note.
 *
 * The name is given in the english notation, where C == Do
 *
 */
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
    static unsigned maxId() const;
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

    static const Pitch C = Pitch(0);
    static const Pitch Cs = Pitch(1);
    static const Pitch Df = Pitch(1);
    static const Pitch D = Pitch(2);
    static const Pitch Ds = Pitch(3);
    static const Pitch Ef = Pitch(3);
    static const Pitch E = Pitch(4);
    static const Pitch F = Pitch(5);
    static const Pitch Fs = Pitch(6);
    static const Pitch Gf = Pitch(6);
    static const Pitch G = Pitch(7);
    static const Pitch Gs = Pitch(8);
    static const Pitch Af = Pitch(8);
    static const Pitch A = Pitch(9);
    static const Pitch As = Pitch(10);
    static const Pitch Bf = Pitch(10);
    static const Pitch B = Pitch(11);
};

class Note : public Pitch
{
private :
    bool silence;
    Fraction duration;
public :
    bool check() const;
    void save(std::ostream& o) const;rest
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

class Figure
{
private :
    struct WeightedNote : public Note
    {
        unsigned weight;
    };
private :
    std::vector<WeightedNote> notes;
public :
    using ConstIterator = std::vector<WeightedNote>::const_iterator;
    using Iterator = std::vector<WeightedNote>::iterator;
    bool check() const;
    void save(std::ostream& o) const;
    static Figure load(std::istream& i);
    static Figure randomInstance();
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Figure fromStream(std::istream& i);

    Figure() = default;
    template<typename Iterator>
    Figure(Iterator begin, Iterator end);
    Figure(const Figure&) = default;
    Figure(Figure&&) = default;
    Figure& operator=(const Figure&) = default;
    ~Figure() = default;


    Iterator begin();
    ConstIterator begin() const;
    Iterator end();
    ConstIterator end() const;
    unsigned nbNotes() const;

    void addNote(const Note& n, unsigned weight = 1);

    const WeightedNote& getNoteAtTime(const Fraction& t) const;
    Fraction getTotalDuration() const;

    std::vector<Fraction> getNotePercentage() const;
};

struct Signature
{
private :
    unsigned number;
    unsigned step;
public :
    bool check() const;
    void save(std::ostream& o) const;
    static Signature load(std::istream& i);
    unsigned id() const;
    static Signature fromId(unsigned hashValue);
    static Signature randomInstance();
    static unsigned maxId() const;
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Signature fromStream(std::istream& i);

    Signature();
    Signature(unsigned number, unsigned step);
    Signature(const Signature&) = default;
    Signature(Signature&&) = default;
    Signature& operator=(const Signature&) = default;
    ~Signature() = default;

    bool operator==(const Signature&) const;
    bool operator!=(const Signature&) const;

    unsigned getNumber() const;
    unsigned getStep() const;
    Fraction measureTime() const;
};


struct Key
{
    Pitch base; ///< Base note of the key
    Pitch mode; ///< Mode of the key

    bool check() const;
    void save(std::ostream& o) const;
    static Key load(std::istream& i);
    unsigned id() const;
    static Key fromId(unsigned hashValue);
    static Key randomInstance();
    static unsigned maxId() const;
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Key fromStream(std::istream& i);

    Key(const Pitch& base = Pitch::C, const Pitch& mode = Pitch::C);
    Key(const Key&) = default;
    Key(Key&&) = default;
    Key& operator=(const Key&) = default;
    ~Key() = default;

    bool operator==(const Key&) const;
    bool operator!=(const Key&) const;

    const std::vector<Pitch>& getNotes() const;
};

class Melody
{
private:
    struct PositionedFigure : public figure
    {
        unsigned pos; ///< Position inside the progession
    };
    using PositionFunction = std::function<unsigned(const Figure&, const Figure&, unsigned)>;
private:
    std::vector<PositionedFigure> figures;
    Key k;
    Signature s;
public :
    using Iterator = std::vector<PositionedFigure>::iterator;
    using ConstIterator = std::vector<PositionedFigure>::const_iterator;

    bool check() const;
    void save(std::ostream& o) const;
    static Melody load(std::istream& i);
    static Melody randomInstance();
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Melody fromStream(std::istream& i);

    Melody(const Key& k, const Signature& s);
    template<typename Iterator>
    Melody(const Key& k, const Signature& s, Iterator begin, Iterator end, const PositionFunction& pf
           = PositionFunction([](const Figure&, const Figure&, unsigned i){return (i+1)%4;}));
    Melody(const Melody&) = default;
    Melody(Melody&&) = default;
    Melody& operator=(const Melody&) = default;
    ~Melody() = default;

    Iterator begin();
    ConstIterator begin() const;
    Iterator end();
    ConstIterator end() const;
    unsigned nbFigures();

    const Key& getKey() const;
    const Signature& getSignature() const;

    void addFigure(const Figure& f, const PositionFunction& pf = PositionFunction([](const Figure&, const Figure&, unsigned i){return (i+1)%4;}));

    Melody transpose(const Key& newKey) const;
};

class Chord
{
private :
    //??????
public :
    bool check() const;
    void save(std::ostream& o) const;
    static Chord load(std::istream& i);
    unsigned id() const;
    static Chord fromId(unsigned hashValue);
    static Chord randomInstance();
    static unsigned maxId() const;
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Chord fromStream(std::istream& i);

    Chord(/*????*/);
    Chord(const Chord&) = default;
    Chord(Chord&&) = default;
    Chord& operator=(const Chord&) = default;
    ~Chord() = default;

    bool operator==(const Chord&) const;
    bool operator!=(const Chord&) const;

    const std::vector<Pitch>& getNotes() const;
};

using ChordProgression = std::vector<Chord>;


#endif // MUSIC_HPP_INCLUDED
