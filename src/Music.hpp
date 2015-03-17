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
    using const_iterator = std::vector<WeightedNote>::const_iterator;
    using iterator = std::vector<WeightedNote>::iterator;
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


    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

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

    std::vector<Pitch> getNotes() const;
};

class Melody
{
private:
    struct PositionedFigure : public figure
    {
        unsigned pos; ///< Position inside the progession
    };
private:
    std::vector<PositionedFigure> figures;
    Key k;
    Signature s;
};

/**
 *@brief output operator for NoteName
 *The format is capital letters for the NoteName
 *@param o the stream to write in
 *@param n the NoteName to write
 *@return o
 */
std::ostream& operator <<(std::ostream& o, NoteName n);

/**
 *@brief input operator for NoteName
 *The format is capital or small letters for the NoteName
 *@param i the stream to read from
 *@param n the NoteName to write in
 *@throw std::domain_error if not a NoteName
 *@return i
 */
std::istream& operator >>(std::istream& i, NoteName& n);

/** @enum Accidental
 *  @brief Describe the accidental of the note.
 *
 * The accidental changes the tone of the note by up to half a tone (called a semitone).
 *
 */
enum class Accidental
{
    Flat,///< Means that we add a semitone
        Sharp,///< Means that we subtract a semitone
        None///< Means that we use the natural note (no accidental)
        };

/**
 *@brief output operator for Accidental
 *The format is ^ for sharp, _ for flat and nothing for none
 *@param o the stream to write in
 *@param a the Accidental to write
 *@return o
 */
std::ostream& operator <<(std::ostream& o, Accidental a);

/**
 *@brief input operator for Accidental
 *The format is ^ for sharp, _ for flat and nothing or = for none
 *@param i the stream to read from
 *@param a the Accidental to write in
 *@return i
 */
std::istream& operator >>(std::istream& i, Accidental& a);

using Octave = unsigned;///<Octave 4 is A 440Hz
#ifdef BOOST
///Fraction class to easily manipulate fractions of beats, measures,...
using Fraction = boost::rational<int>;
#else
#warning "Not using boost is a problem"
using Fraction = float;
#endif

/**@brief Fully describes a note (name + accidental)
 */
struct CompleteNoteName
{
    NoteName m_name; ///< Name of the note (C,D,...)
    Accidental m_modifier;///< Accidental of the note (sharp, flat,none)
    /** @brief Constructor for CompleteNoteName
     *
     *
     * @param n  Name of the note (C,D,...)
     * @param mod Accidental of the note (sharp, flat,none)
     @note done
    */
    CompleteNoteName(NoteName n=NoteName::C, Accidental mod=Accidental::None);
    /**
     *@brief Comparison operator for CompleteNoteName
     @param k is the key in which we transpose the chord progression
     @note done
    */
    bool operator==(const CompleteNoteName& o) const __attribute__((pure));
    /**
     *@brief Difference operator for CompleteNoteName
     @note done
    */
    bool operator!=(const CompleteNoteName& o) const __attribute__((pure));
};

/**
 *@brief output operator for CompleteNoteName
 *The format is as follow : the modifier (if any) followed by the NoteName
 *@param o the stream to write in
 *@param n the CompleteNoteName to write
 *@return o
 */
std::ostream& operator <<(std::ostream& o, const CompleteNoteName& n);

/**
 *@brief input operator for CompleteNoteName
 *The format is as follow : the modifier (if any) followed by the NoteName
 *@param i the stream to read from
 *@param n the CompleteNoteName to write in
 *@return i
 */
std::istream& operator >>(std::istream& i, CompleteNoteName& n);

/** @brief Describe a note with its absolute pitch
 */
struct Pitch : public CompleteNoteName
{
    Octave m_octave; ///< octave of the note (absolute height of the note)
    /**
     *@brief Construtor for Pitch with a CompleteNotename
     @note done
    */
    Pitch(const CompleteNoteName& n, Octave o = 4);
    /**
     *@brief Construtor for Pitch with full info
     *If the note is A Flat, and octave o, it means it is a G Sharp at octave o
     @note done
    */
    Pitch(NoteName n = NoteName::C, Accidental mod=Accidental::None, Octave o = 4);
    /**
     *@brief Adds half tones to pitch
     *@param nb the number of half tones to add
     *@return *this
     *@throw std::domain_error if the result is bellow the lowest representable note
     *done
     */
    Pitch& operator+=(int nb);
    /**
     *@brief Creates new Pitch by adding nb half tones to this one
     *@param nb the number of half tones to add
     *@throw std::domain_error if the result is bellow the lowest representable note
     *@return a Pitch which sound is nb half tones higher
     *done
     */
    Pitch operator+(int nb) const __attribute__((pure));
    /**
     *@brief Substract half tones to pitch
     *@param nb the number of half tones to substract
     *@return *this
     *@throw std::domain_error if the result is bellow the lowest representable note
     *done
     */
    Pitch& operator-=(int nb);
    /**
     *@brief Creates new Pitch by substracting nb half tones to this one
     *@param nb number of half tones to substract
     *@return a Pitch which sound is nb half tone lower
     *@throw std::domain_error if the result is bellow the lowest representable note
     *done
     */
    Pitch operator-(int nb) const __attribute__((pure));
    /**
     *@brief Comparison operator for Pitch
     *done
     */
    bool operator==(const Pitch& o) const __attribute__((pure));
    /**
     *@brief Difference operator for Pitch
     *done
     */
    bool operator!=(const Pitch& o) const __attribute__((pure));
    /**@brief Returns the name of the note, ommiting the octave
     * @return a complete note name
     */
    CompleteNoteName getNoteName() const __attribute__((pure));
};

/**
 *@brief output operator for Pitch
 *The format is as follow : the CompleteNoteName followed by (Octave-4) ' or (4-Octave) ,
 *Without specification, a note is assumed to be in the forth octave.
 *The string "A" thus refer to a A 440Hz.
 *From this reference, one can add or subtract octave. The character "'" corresponds to
 *the addition of one octave, and "," to the subtraction.
 *For example, "_G,," represents a G flat in the second octave (4-2)
 and "B'''" represents a B in the seventh octave (4+3)
 *@param o the stream to write in
 *@param n the Pitch to write
 *@return o
 */
std::ostream& operator <<(std::ostream& o, const Pitch& n);

/**
 *@brief input operator for Pitch
 *The format is as follow : the CompleteNoteName followed by (Octave-4) ' or (4-Octave) ,
 *Without specification, a note is assumed to be in the forth octave.
 *The string "A" thus refer to a A 440Hz.
 *From this reference, one can add or subtract octave. The character "'" corresponds to
 *the addition of one octave, and "," to the subtraction.
 *For example, "_G,," represents a G flat in the second octave (4-2)
 and "B'''" represents a G in the seventh octave (4+3)
 *If the pitch is A,,,,,, which does not exist, the parsing stops at A,,,, as any standard operator >> behaves
 *@param i the stream to read from
 *@param n the Pitch to write in
 *@return i
 */
std::istream& operator >>(std::istream& i, Pitch& n);

/**
 *@brief Describe a note as a musical event (pitch + duration)
 */
struct Note : public Pitch
{
    Fraction m_duration; ///<Duration of the note, as a fraction of a beat
    /**
     *@brief Constructor for note based on pitch
     *@throw std::domain_error if the duration is non positive
     *@note done
     */
    Note(const Pitch& n = Pitch(), const Fraction& duration = 1);
    /**
     *@brief Constructor for note with full info
     *@throw std::domain_error if the duration is non positive
     *@note done
     */
    Note(NoteName n, Accidental mod=Accidental::None, Octave o = 4, const Fraction& duration = Fraction(1));
    /**
     *@brief Comparison operator for Note
     *done
     */
    bool operator==(const Note& o) const __attribute__((pure));
    /**
     *@brief Difference operator for Note
     *done
     */
    bool operator!=(const Note& o) const __attribute__((pure));
    /**
     * Correspondance between notes and integers (respecting the order)
     * Useful in MusicRules.cpp as in Music.cpp
     **/
    static unsigned noteToHalfTone(const CompleteNoteName& n) __attribute__((const));
};

/**
 *@brief output operator for Note
 *The format is as follow : the Pitch followed by num/denom where num/denom is the fraction of beat.
 If num or denum is 1, they do not appear.
 *@param o the stream to write in
 *@param n the Note to write
 *@return o
 *@note done
 */
std::ostream& operator <<(std::ostream& o, const Note& n);

/**
 *@brief input operator for Note
 *The format is as follow : the Pitch followed by the fraction of beat.
 If the denominator is not given, it is supposed to be 2.
 The details of the possibilities are given in the ABC notations.
 The web page of the examples is http://abcnotation.com/examples#Note-lengths
 *@param i the stream to read from
 *@param n the Note to write in
 *@return i
 *@note done
 */
std::istream& operator >>(std::istream& i, Note& n);

/** @brief Describe a musical bar.
 *
 * This class can only describe monophonic melodies
 * (at each timestep, there must be at most one note playing).
 * We will assume there is 4 beats per measure
 */
class Measure
{
public :
    //Constructor must verify Measure invariants

    using Position = Fraction;///<The Position type is a Fraction of Measure
    using ConstIterator = std::map<Position, Note>::const_iterator;///<Allows to iterate on the notes of the measure

    //Constructor

    /**
     *@brief Constructor. Creates an empty Measure
     *@param binary indicates whether the measure is binary
     *@param duration the duration of the measure.
     *@throw "Duration is not 4" if duration != 4 (we only deal with four beat measures atm)
     *@note done without exception
     */
    Measure(bool binary = true, unsigned duration = 4);

    /**
     *@brief returns an iterator at the beginning of measure
     *@note done
     */
    ConstIterator begin() const __attribute__((pure));
    /**
     *@brief returns the end iterator of the measure
     *@note done
     */
    ConstIterator end() const __attribute__((pure));
    /**
     *@brief Gets the duration of the measure
     *@note done
     */
    unsigned getDuration() const __attribute__((pure));

    /**@brief Resize the measure
     * The resizing operation is not trivial, it changes the balance of the music
     * @param d the number of beats inside the measure
     * @throws std::domain_error if the resizing is going to cause at least one note to be deleted
     * @note ignore this function we only have 4 beat measures
     */
    void changeDuration(unsigned d);
    /**
     *@brief returns if the measure is binary
     *@note done
     */
    bool isBinary() const __attribute__((pure));

    /**
     *@brief sets the binary type of the measure
     *@note I do not see the use for this function
     *@note done
     */
    void setBinary(bool b);



    /** @brief Get the note played at a given position of the bar
     *
     * By assumption, as we only describe monophonic melodies, this note is unique.
     * Also note that the position is not necessarily the beginning of a note (notes are not
     * ponctual events, thus a note is played for a strictly positive amount of time)
     * @param p The position inside the bar (as a fraction of the bar).We must have 0<=p<=1. [CHANGE WARNING]
     * @return The note being played at position p.
     * @throw std::domain_error If no note is played at position p (ie there is only rest)
     * @throw std::domain_error if !(0<=p<=1)
     */
    ConstIterator getNoteBeingPlayedAt(const Position& p) const __attribute__((pure));

    /**
     *@brief Adds note to measure
     *@param p the position in the measure
     *@param n the note to add
     * Removed feature : if there is twice the same note right after the other, it combines both notes
     *@throw std::runtime_error "already a note there"
     *@throw std::domain_error "Position invalid"
     *@note done
     */
    void addNote(const Position& p, const Note& n);

    /**
     *@brief Comparison operator for Measure
     *@note done
     */
    bool operator==(const Measure& o) const __attribute__((pure));

    /**
     *@brief Difference operator for Measure
     *@note done
     */
    bool operator!=(const Measure& o) const __attribute__((pure));
private :

    std::map<Position, Note> m_starts; ///< Note starting at each position
    unsigned m_duration; ///< Duration of the measure (in number of beats).
    bool m_binary;///<True iff the measure is binary, false if it is ternary.


};

/**
 *@brief output operator for Measure
 *The format is as follows : The notes, separated by spaces. rests are written zFractionofbeat
 *@param o the stream to write in
 *@param m the Measure to write
 *@return o
 *@note done
 */
std::ostream& operator <<(std::ostream& o, const Measure& m);

/**
 *@brief input operator for Measure
 *The format is as follows : The notes, separated by spaces (or not). rests are written zFractionofbeat
 *@param i is the stream to read from
 *@param m the Measure to write in
 *@return i
 *@note done
 **/
std::istream& operator >>(std::istream& i, Measure& m);

/**@brief Describe the key of a melody
 *
 * The key is composed of the name of the base note (first of the scale)
 * and the mode, which defines the intervals between the notes.
 */

struct Key
{
    CompleteNoteName m_base; ///< Base note of the key
    NoteName m_mode; ///< Mode of the key

    /**
     *@brief Constructor from a CompleteNoteName for the base and a NoteName for the mode
     *@note done
     */
    Key(const CompleteNoteName& base=CompleteNoteName(), const NoteName& mode = NoteName::C);

    /**
     *@brief Transform a key into the list of its seven notes
     *@return A vector of CompleteNoteName containing the seven notes of the key
     *@note done
     */
    std::vector<CompleteNoteName> getNotes() const;

    /**
     * @brief Change the note (given as a reference) to get the notation corresponding to the key
     * @param c is the note to normalize
     * @note work in progress
     **/
    void normalize(CompleteNoteName& c) const;
    /**
     *@brief equality operator for Key
     *@note done
     */
    bool operator==(const Key& k) const __attribute__((pure));

    /**
     *@brief inequality operator for Key
     *@note done
     */
    bool operator!=(const Key& k) const __attribute__((pure));

};

/**
 *@brief output operator for Key
 *The format is as shown in ABC notation
 Link is : http://abcnotation.com/wiki/abc:standard:v2.1#kkey
 *@param o the stream to write in
 *@param k the Key to write
 *@return o
 */
std::ostream& operator <<(std::ostream& o, const Key& k);

/**@brief input parser for Key
 *The format is as shown in ABC notation
 Link is : http://abcnotation.com/wiki/abc:standard:v2.1#kkey
 *@param str the string to read from
 *@return the Key to write in
 *@throw std::runtime_error
 *@note Replaces operator >> for key
 **/
Key readKey(const std::string& str);

/** @brief Describe a chord
 *
 * A chord is characterized by a base note, a duration (fraction of the measure)
 * and some alteration.
 */

struct Chord
{
    /** @enum ChordType
     *  @enum SeventhType
     *  @brief Describe the nature of the chord
     *
     * By default, a chord is a triad composed of a base note, a third, and a fifth over it.
     */

    /**
     *@brief Describes the type of the third note in the chord
     **/
    enum class ChordType
    {
        Minor, ///< Describes a minor chord, meaning that the first third is a minor third
            Major, ///< Describes a major chord, meaning that the first third is a major third
            Sus4, ///< Describes a suspended fourth chord, meaning that the third is replaced by a forth
            Sus2,///< Describes a suspended second chord, meaning that the third is replaced by a second
            Dim, ///< Describes a diminished chord, composed of a minor third and a diminished fifth
            Aug ///< Describes a augmented chord, in which the fifth is augmented
            };
    Fraction m_duration;///< Hold the duration of the chord, in fraction of the bar it is played in. SHOULD BE DELETED ?
    CompleteNoteName m_base; ///< Name of the base name of the note
    /**
     *@brief Describes the Type of the fourth note of the chord
     **/
    enum class SeventhType
    {
        Major,
            Minor,
            Dim,
            None
            };
    SeventhType m_seventh; ///< if it's not None, the seventh is added according to this value (Dim = double Flat)
    ChordType m_type; ///<Type of the chord

    /**@brief transform a chord into notes
       @return vector of size 3 (or 4) containing the notes of the chord
       @note done
    */
    std::vector<CompleteNoteName> getNotes() const __attribute__((pure));

    /**@brief Constructor with parameters
     *
     * @param d Duration of the chord, in fraction of the bar it is played in.
     * @param b Name of the base name of the note
     * @param s Type of the seventh
     * @param c Type of the chord
     * @note done
     */
    Chord(Fraction d = 4, const CompleteNoteName& b= CompleteNoteName(), const SeventhType& s = SeventhType::None, const ChordType& c = ChordType::Major);

    /**
     *@brief Equality operator for Chord
     *@note done
     */
    bool operator==(const Chord& o) const __attribute__((pure));
    /**
     *@brief Inequality operator for Chord
     *@note done
     */
    bool operator!=(const Chord& o) const __attribute__((pure));

    using ChordID = unsigned;
    explicit Chord(ChordID id);
    explicit operator ChordID() const __attribute__((pure));
    static const unsigned nbChords = 6*4*12;
    static const std::vector<CompleteNoteName>& getNotes(ChordID c);

};

/**
 *@brief output operator for Chord
 *@param o the stream to write in
 *@param c the Chord to write
 *The format follows the guitar chord format
 *@return o
 */
std::ostream& operator <<(std::ostream& o, const Chord& c);

/**
 *@brief parser for chord
 *The format follows the guitar chord format
 *@param s the string to parse
 *@return the chord read
 *@throw std::runtime_error
 */
Chord readChord(const std::string& s);

///ChordProgression is the class used to give the set of chords used
using ChordProgression = std::vector<Chord>;

/**
 *@brief output operator for ChordProgression
 *The format is as follows : Chord1;Chord2;...LastChord;
 *@param o the stream to write in
 *@param c the ChordProgression to write
 *@return o
 */
std::ostream& operator <<(std::ostream& o, const ChordProgression& c);

/**
 *@brief parser for ChordProgression
 *The format is as follows : Chord1;Chord2;...LastChord;
 *@param str the string to read from
 *@return the ChordProgression
 *@throw std::runtime_error
 */
ChordProgression readChordProgression(const std::string& str);

/**
 *@brief change the base notes of a chord progression
 *@param i is the number of half tones to add
 *@param cp is the chordprogression to change
 *@param k is the key in which we transpose the chord progression
 *@return the transposed ChordProgression
 **/

ChordProgression moveCP(const ChordProgression& cp, int i, const Key& k);

/**@brief Describes a melody
 *
 * A melody is simply a set of bars. Invariants such as key may be checked (although they are by no means required)
 * Carefull : If some notes of the key are sharp, you still need to put Accidental::Sharp in these notes.
 *
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=const"
class Melody
{
public:

    using ConstIterator = std::vector<Measure>::const_iterator;///<Allows to iterate on the measures of the Melody
    //Constructor

    /*! @brief Constructor with initializations.
     *\param v The vector of measures composing the melody
     *\param k The key of the melody
     */
    Melody(const std::vector<Measure>& v,Key k, unsigned tempo = 120);

    /**@brief Creates an empty melody
     */

    Melody(const Key& k = Key(NoteName::C));


    /*! @brief Returns the measures of the melody
     *\return A vector of measures
     *\note should not be used
     *@note done
     */
    std::vector<Measure> convertToMeasures() const __attribute__((pure));

    /*! Returns the key of the melody
     *\return The key of the melody
     */
    const Key& getKey() const __attribute__((pure));

    // Set_functions


    /**
     *@brief Returns a measure iterator at the beginning of the Melody
     *@note done
     */
    ConstIterator begin() const __attribute__((pure));
    /**
     *@brief Returns a measure iterator at the end of the Melody
     *@note done
     */
    ConstIterator end() const __attribute__((pure));
    /**
     *@brief Returns The number of measures in the Melody
     *@return The number of measures in the Melody
     **/
    unsigned size() const __attribute__((pure));

    /** @brief Transpose a melody into a given key
     *
     *
     * @param k The target key
     * @return A new melody object, in key k
     */
    Melody transpose(const Key& k) const __attribute__((pure));

    /**@brief returns if the melody is binary (looks at first measure)
     *@throw runtime_error("Unknown");
     **/
    bool isBinary() const;

    /**
     *@brief Equality operator for Melody
     *@note done
     */
    bool operator==(const Melody& o) const __attribute__((pure));
    /**
     *@brief Inequality operator for Melody
     *@note done
     */
    bool operator!=(const Melody& o) const __attribute__((pure));

    /**
     *@brief Adds a Chord Progression to a file
     *@param out is the stream to write the ChordProgression to
     *@param chords is the ChordProgression to add
     **/
    std::ostream& createABCChordFile(std::ostream& out, const ChordProgression& chords) const;

    unsigned m_BPM;
private:
    std::vector<Measure> m_measures;///< The list of measures of the melody
    Key m_k; ///< The key of the melody
};

#pragma GCC diagnostic pop
/**
 *@brief output operator for Melody
 *The format is as follows :
 *M : 4/4 if binary, M : 12/8 if not
 *K : Key
 *L : 1/4
 *Measure1 | Measure2 | ...
 *@param o the stream to write in
 *@param m the Melody to write
 *@return o
 */
std::ostream& operator <<(std::ostream& o, const Melody& m);

/**
 *@brief parser for Melody
 *The format is the ABC format. We ignore |, :, >, ... when reading something that is not a header
 *@param str the string to read from
 *@return the melody read
 */

Melody readMelody(const std::string& str);

#endif // MUSIC_HPP_INCLUDED
