/**
 * @file   Signature.hpp
 * @author  Julien and Nicolas
 * @date   Wed Mar 18 00:29:40 2015
 *
 * @brief  Describe the signature structure
 *
 *
 */
#ifndef SIGNATURE_HPP_INCLUDED
#define SIGNATURE_HPP_INCLUDED

#include <iostream>
#include "Utilities.hpp"

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
    static unsigned maxId();
    std::ostream& operator<<(std::ostream& o) const;
    std::ostream& operator>>(std::istream& i);
    static Signature fromStream(std::istream& i);

    /** @author Valentin
     * Default constructor for a signature, 4 quarter notes a bar
     * Implemented and tested (alcinos)
     * @return a default 4/4 signature
     */
    Signature();

    /** @author Valentin
     * Basic constructor for any signature
     * Implemented not tested
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
     * Usual equality testing operator
     * Implemented not tested
     * @return true iff both Signatures are equal
     */ 
    bool operator==(const Signature&) const;

    /** @author Valentin
     * Usual difference testing operator
     * Implemented not tested
     * @return true iff both Signatures are different
     */ 
    bool operator!=(const Signature&) const;

    /** @author Valentin
     * Getter for the number parameter
     * Implemented not tested
     * @return the number parameter of the signature
     */ 
    unsigned getNumber() const;

    /** @author Valentin
     * Getter for the step parameter
     * Implemented not tested
     * @return the step parameter of the signature
     */ 
    unsigned getStep() const;

    /** @author Valentin
     * Function to get a relative duration of a measure in this signature (4/4 has a duration of 1)
     * Implemented not tested
     * @return the relative duration of a measure
     */
    Fraction measureTime() const;

};

#endif
