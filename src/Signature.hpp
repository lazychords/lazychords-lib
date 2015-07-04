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
     * 
     * @return a default 4/4 signature
     */
    Signature();

    /** @author Valentin
     * Basic constructor for any signature
     * 
	 * @param number is the number of steps in a bar
	 * @param step defines the step value (most often 4 for binary, 8 for ternary)
     * @return a number/step signature
     */
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

#endif
