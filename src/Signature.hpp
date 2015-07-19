/**
 * @file   Signature.hpp
 * @author  Julien and Nicolas and Valentin
 * @date   Wed Mar 18 00:29:40 2015
 *
 * @brief  Describe the signature structure
 *
 *
 */
#ifndef SIGNATURE_HPP_INCLUDED
#define SIGNATURE_HPP_INCLUDED

#include <iostream>
#include <cmath>
#include "Utilities.hpp"

struct Signature
{
private :
    unsigned number;
    unsigned step;
public :
    static constexpr unsigned maxStep = 64;
    static constexpr unsigned maxNumber = 128;

    /** @author Valentin
     *  @brief Sanity check for Signature
     *  Implemented
     *  @return true iff the signature is valid
     */
    bool check() const;
    void save(std::ostream& o) const;
    static Signature load(std::istream& i);

    /** @author Valentin
        @brief Gets the id associated to the Signature
        Implemented not tested
        @return unsigned value corresponding to the unique id of the signature
     */
    unsigned id() const;
    /** @author Valentin
        @brief Creates a Signature given an ID
        Implemented not tested
        @param unsigned value that should be a valid ID for Signature
        @return the Signature corresponding to the ID
     */
    static Signature fromId(unsigned hashValue);
    /** @author Valentin
        @brief Creates a random Signature among all the possible Signatures
        Implemented not tested
        @return a random Signature
     */
    static Signature randomInstance();
    /** @author Valentin
        @brief Gets the greatest ID for Signature
        Implemented not tested
        @return the maximum of the different ID
     */
    static constexpr unsigned maxId();
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Signature fromStream(std::istream& i);
    /** @author Valentin
     * @brief Default constructor for a signature, 4 quarter notes a bar
     * Implemented and tested (alcinos)
     * @return a default 4/4 signature
     */
    Signature();

    /** @author Valentin
     * @brief Basic constructor for any signature
     * Implemented and tested
	 * @param number is a positive number of steps in a bar
	 * @param step defines the step value (most often 4 for binary, 8 for ternary), must be a power of 2
     * @return a number/step signature
     */
    Signature(unsigned number, unsigned step);
    Signature(const Signature&) = default;
    Signature(Signature&&) = default;
    Signature& operator=(const Signature&) = default;
    ~Signature() = default;

    /** @author Valentin
     * @brief Usual equality testing operator
     * Implemented and tested (alcinos)
     * @return true iff both Signatures are equal
     */
    bool operator==(const Signature&) const;

    /** @author Valentin
     * @brief Usual difference testing operator
     * Implemented and tested (alcinos)
     * @return true iff both Signatures are different
     */
    bool operator!=(const Signature&) const;

    /** @author Valentin
     * @brief Getter for the number parameter
     * Implemented not tested
     * @return the number parameter of the signature
     */
    unsigned getNumber() const;

    /** @author Valentin
     * @brief Getter for the step parameter
     * Implemented not tested
     * @return the step parameter of the signature
     */
    unsigned getStep() const;

    /** @author Valentin
     * @brief Function to get a relative duration of a measure in this signature (4/4 has a duration of 1)
     * Implemented not tested
     * @return the relative duration of a measure
     */
    UFraction measureTime() const;

};

#endif
