/**
 * @file   Chord.hpp
 * @author  Julien and Nicolas
 * @date   Wed Mar 18 00:41:19 2015
 *
 * @brief  Describe the Chord structure
 *
 *
 */
#ifndef CHORD_HPP_INCLUDED
#define CHORD_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "Pitch.hpp"

class Chord
{
public:
    enum Type5 {MAJ5=0, MIN5, AUG5, DIM5, SUS45};
    enum Type7 {NONE=0,MAJ7,MIN7,DIM7};
private :
    Pitch base;
    Type5 fifth;
    Type7 seventh;
public :
    bool check() const;
    void save(std::ostream& o) const;
    static Chord load(std::istream& i);

    /**@author Valentin
     *@brief Follows the @ref ConceptId "Id" concept
     *@todo Test
    **/
    unsigned id() const;

    /**@author Valentin
     *@ref ConceptId
     *@todo Test
    **/
    static Chord fromId(unsigned hashValue);

    /**@author Valentin
     *@ref ConceptId
     *@todo Test
    **/
    static Chord randomInstance();

    /** @author Valentin
     * @ref ConceptId
     * @todo Test
     */
    static unsigned maxId();

    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Chord fromStream(std::istream& i);

    /** @author Valentin
     * Basic constructor for 3-notes chord
     * @param The tonic note of the chord
     * @param The quality of the chord (Major, Minor, Diminished, Augmented)
     * @return a Chord object corresponding to its parameters
     * @todo Test
     */
    Chord(const Pitch&, const Type5&);
    /** @author Valentin
     * Basic constructor for 4-notes chord
     * @param The tonic note of the chord
     * @param The quality of the chord (Major, Minor, Diminished, Augmented)
     * @param The quality of the seventh (Major7, 7, Dim7)
     * @return a Chord object corresponding to its parameters
     * @todo Test 
     */
    Chord(const Pitch&, const Type5&, const Type7&);
    Chord(const Chord&) = default;
    Chord(Chord&&) = default;
    Chord& operator=(const Chord&) = default;
    ~Chord() = default;

    /** @author Valentin
     * Usual equality testing operator
     * @return true iff both Chords are equal
     * @todo Test
     */
    bool operator==(const Chord&) const;
    /** @author Valentin
     * Usual difference testing operator
     * @return true iff both Chords are different
     * @todo Test
     */
    bool operator!=(const Chord&) const;

    /** @author Valentin
     * Transforms a chord into its notes (no octava taken into account)
     * @return a vector of notes containing the 3 (or 4) notes of the chord
     * @todo Test
     */
    std::vector<Pitch> getNotes() const;
};

using ChordProgression = std::vector<Chord>;


#endif
