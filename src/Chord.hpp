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

#include "Pitch.hpp"

class Chord
{
public:
    enum Type5 {MAJ5=0, MIN5, AUG5, DIM5, SUS45};
    static constexpr unsigned type5size = 5;
    enum Type7 {NONE=0,MAJ7,MIN7,DIM7};
    static constexpr unsigned type7size = 4;
private :
    Pitch base;
    Type5 fifth;
    Type7 seventh;

    static std::vector<std::vector<Pitch> > getAllChordsNotes();
    std::vector<Pitch> getChordNotes() const;
public :
    void check() const;
    void save(std::ostream& o) const;
    static Chord load(std::istream& i);

    /**@author Valentin
     *@brief Follows the @ref ConceptId "Id concept"
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
    static constexpr unsigned maxId();

    static Chord fromStream(std::istream& i);

    /**@author Valentin
     * @brief Basic constructor for 3-notes chord
     * @param p is the tonic note of the chord
     * @param t is the quality of the chord (Major, Minor, Diminished, Augmented)
     * @return a Chord object corresponding to its parameters
     * @todo Test
     */
    Chord(const Pitch& p, const Type5& t);
    /** @author Valentin
     * @brief Basic constructor for 4-notes chord
     * @param p is the tonic note of the chord
     * @param t5 is the quality of the chord (Major, Minor, Diminished, Augmented)
     * @param t7 is the quality of the seventh (Major7, 7, Dim7)
     * @return a Chord object corresponding to its parameters
     * @todo Test
     */
    Chord(const Pitch& p, const Type5& t5, const Type7& t7);
    Chord(const Chord&) = default;
    Chord(Chord&&) = default;
    Chord& operator=(const Chord&) = default;
    ~Chord() = default;

    /** @author Valentin
     * @brief Usual equality testing operator
     * @return true iff both Chords are equal
     * @todo Test
     */
    bool operator==(const Chord&) const;
    /** @author Valentin
     * @brief Usual difference testing operator
     * @return true iff both Chords are different
     * @todo Test
     */
    bool operator!=(const Chord&) const;

    /** @author Valentin
     * @brief Transforms a chord into its notes (no octava taken into account)
     * @return a vector of notes containing the 3 (or 4) notes of the chord
     * @todo Test
     */
    const std::vector<Pitch>& getNotes() const;
};

using ChordProgression = std::vector<Chord>;

std::ostream& operator<<(std::ostream& o, const Chord& c);
std::ostream& operator>>(std::istream& i, Chord& c);

#endif
