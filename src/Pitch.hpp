/**
 * @file   Pitch.hpp
 * @author  Julien and Nicolas
 * @date   Tue Mar 17 17:13:48 2015
 *
 * @brief  This file defines the Pitch structure
 *
 *
 */
#ifndef PITCH_HPP_INCLUDED
#define PITCH_HPP_INCLUDED

#include "Utilities.hpp"

class Pitch;

class Pitch
{
private:
    unsigned halfTone;///<Number of semitones from C. Must be between 0 and 11;

    //we declare this class as friend to allow serialization
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

public:
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
    static Pitch load(std::istream& i);
    unsigned id() const;
    static Pitch fromId(unsigned hashValue);
    static constexpr unsigned maxId() { return 12; }

    /** @author alcinos
     * Generate a random valid instance of the pitch structure
     * Implemented
     * @return A random instance
     */
    static Pitch randomInstance();
    std::ostream& operator<<(std::ostream& o) const;
    std::istream& operator>>(std::istream& i);
    static Pitch fromStream(std::istream& i);

    /** @author alcinos
     * Construction from distance to base note
     * Implemented
     * @param halfTones distance in semitones from the base note C.
     */
    explicit Pitch(unsigned halfTones = 0);
    Pitch(const Pitch&) = default;
    Pitch(Pitch&&) = default;
    Pitch& operator=(const Pitch&) = default;
    ~Pitch() = default;

    /** @author alcinos
     * Usual equality testing operator
     * Implemented
     * @return true iff both Pitches are equal
     */
    bool operator==(const Pitch&) const;
    /** @author
     * Usual difference testing operator
     * Implemented
     * @return true iff both Pitches are different
     */
    bool operator!=(const Pitch&) const;

    /** @author alcinos
     * Add some semitones to the current note.
     * Note that the addition occurs modulo the number of semitones in the scale (12)
     * Implemented
     * @param halfTones number of semitones to add
     * @return a reference to the resulting object
     */
    Pitch& operator+=(int halfTones);

    /** @author alcinos
     * Subtract some semitones to the current note.
     * Note that the subtraction occurs modulo the number of semitones in the scale (12)
     * Implemented
     * @param halfTones number of semitones to subtract
     * @return a reference to the resulting object
     */
    Pitch& operator-=(int halfTones);

    /** @author alcinos
     * Binary addition operator
     * Note that the addition occurs modulo the number of semitones in the scale (12)
     * Implemented
     * @param halfTones number of semitones to add
     * @return the resulting object
     */
    Pitch operator+(int halfTones) const;

    /** @author alcinos
     * Binary subtractio operator
     * Note that the subtraction occurs modulo the number of semitones in the scale (12)
     * Implemented
     * @param halfTones number of semitones to subtract
     * @return the resulting object
     */
    Pitch operator-(int halfTones) const;


    //the following are definitions of the natural notes of the scales.
    //s stands for "sharp" and f for "flat"
    static const Pitch C;
    static const Pitch Cs;
    static const Pitch Df;
    static const Pitch D;
    static const Pitch Ds;
    static const Pitch Ef;
    static const Pitch E;
    static const Pitch F;
    static const Pitch Fs;
    static const Pitch Gf;
    static const Pitch G;
    static const Pitch Gs;
    static const Pitch Af;
    static const Pitch A;
    static const Pitch As;
    static const Pitch Bf;
    static const Pitch B;
};


#endif
