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


class Note : public Pitch
{
private :
    bool silence;
    Fraction duration;
public :
    /**@author alcinos 
     * Sanity check of the class
     * Implemented
     * @return true iff the class's invariants are met
     */
    bool check() const;
    void save(std::ostream& o) const;
    static Note load(std::istream& i);
    static Note randomInstance();
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Note fromStream(std::istream& i);

    /** @author alcinos 
     * Construction from a duration and a boolean indicating whether the note is a rest
     * This constructor is typically used to construct rests
     * If the rest parameter is set to false, then it creates a default note (C)
     * Implemented
     * @param duration the duration of the note, in fraction of beats
     * @param rest whether the note is a rest
     */
    Note(const Fraction& duration = 1, bool rest = true);

    /**@author alcinos 
     * Construction from distance to base note and a duration 
     * Implemented
     * @param halfTones distance in semitones from the base note C. 
     * @param duration the duration of the note, in fraction of beats
     */
    Note(unsigned halfTones, const Fraction& duration = 1);

    /**@author alcinos 
     * Construction from a pitch and a duration 
     * Implemented
     * @param p pitch of the note
     * @param duration the duration of the note, in fraction of beats
     */
    Note(const Pitch& p, const Fraction& duration);
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
    const Fraction& getDuration() const;

    /** @author alcinos
     * Setter for the duration 
     * Implemented
     * @param d a positive fraction representing, in fraction of beats, the new duration of the note
     */
    void changeDuration(const Fraction& d);

};

#endif
