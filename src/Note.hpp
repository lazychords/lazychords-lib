/**
 * @file   Note.hpp
 * @author  Julien and Nicolas
 * @date   Wed Mar 18 00:19:30 2015
 *
 * @brief  Describe the Note structure
 *
 *
 */
#ifndef NOTE_HPP_INCLUDED
#define NOTE_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "Utilities.hpp"
#include "Pitch.hpp"


class Note : private Pitch
{
private :
    bool silence;
    UFraction duration;

    //we declare this class as friend to allow serialization
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

public :
    /**@author alcinos
     * Sanity check of the class
     * Implemented
     * @return true iff the class's invariants are met
     */
    bool check() const;

    /** @author alcinos
     * Serialize the object to the given output stream
     * Behind the scenes, it relies on Boost's serialization mechanisms
     * Implemented
     * @param o the output stream
     */
    void save(std::ostream& o) const;
    /** @author alcinos
     * Deserialization of an object written on the input stream
     * Implemented
     * @param i input stream containing the object to read
     * @return a fresh object
     */
    static Note load(std::istream& i);

    /** @author alcinos
     * Generate a random valid instance of the Note structure
     * Note that the duration of the note is bounded to 4 beats
     * Implemented
     * @return A random instance
     */
    static Note randomInstance();
    std::ostream& operator<<(std::ostream& o) const;
    std::istream& operator>>(std::istream& i);
    static Note fromStream(std::istream& i);

    /** @author alcinos
     * Construction from a duration and a boolean indicating whether the note is a rest
     * This constructor is typically used to construct rests
     * If the rest parameter is set to false, then it creates a default note (C)
     * Implemented
     * @param duration the duration of the note, in fraction of beats
     * @param rest whether the note is a rest
     */
    explicit Note(const UFraction& duration = 1, bool rest = true);

    /**@author alcinos
     * Construction from distance to base note and a duration
     * Implemented
     * @param halfTones distance in semitones from the base note C.
     * @param duration the duration of the note, in fraction of beats
     */
    explicit Note(unsigned halfTones, const UFraction& duration = 1);

    /**@author alcinos
     * Construction from a pitch and a duration
     * Implemented
     * @param p pitch of the note
     * @param duration the duration of the note, in fraction of beats
     */
    explicit Note(const Pitch& p, const UFraction& duration = 1);
    Note(const Note&) = default;
    Note(Note&&) = default;
    Note& operator=(const Note&) = default;
    ~Note() = default;

    /** @author alcinos
     * Usual equality testing operator
     * Implemented
     * @return true iff both Notes are equal
     */
    bool operator==(const Note&) const;
    /** @author
     * Usual difference testing operator
     * Implemented
     * @return true iff both Notes are different
     */
    bool operator!=(const Note&) const;

    /** @author alcinos
     * Add some semitones to the current note.
     * Note that the addition occurs modulo the number of semitones in the scale (12)
     * Implemented
     * @param halfTones number of semitones to add
     * @return a reference to the resulting object
     */
    Note& operator+=(int halfTones);

    /** @author alcinos
     * Subtract some semitones to the current note.
     * Note that the subtraction occurs modulo the number of semitones in the scale (12)
     * Implemented
     * @param halfTones number of semitones to subtract
     * @return a reference to the resulting object
     */
    Note& operator-=(int halfTones);

    /** @author alcinos
     * Binary addition operator
     * Note that the addition occurs modulo the number of semitones in the scale (12)
     * Implemented
     * @param halfTones number of semitones to add
     * @return the resulting object
     */
    Note operator+(int halfTones) const;
    /** @author alcinos
     * Binary subtractio operator
     * Note that the subtraction occurs modulo the number of semitones in the scale (12)
     * Implemented
     * @param halfTones number of semitones to subtract
     * @return the resulting object
     */
    Note operator-(int halfTones) const;

    /** @author alcinos
     * getter for the rest parameter
     * Implemented
     * @return true iff the note is a rest
     */
    bool isRest() const;
    /** @author alcinos
     * Simple getter for the duration
     * Implemented
     * @return the duration of the note
     */
    const UFraction& getDuration() const;

    /** @author alcinos
     * Setter for the duration
     * Implemented
     * @param d a positive fraction representing, in fraction of beats, the new duration of the note
     */
    void changeDuration(const UFraction& d);

};

#endif
