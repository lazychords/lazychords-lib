/**
 * @file   Melody.hpp
 * @author  <nicolas@archimede>
 * @date   Wed Mar 18 00:37:15 2015
 * 
 * @brief  Describe the melody structure
 * 
 * 
 */
#ifndef MELODY_HPP_INCLUDED
#define MELODY_HPP_INCLUDED

#include "Figure.hpp"
#include "Key.hpp"
#include "Signature.hpp"
#include <functional>
#include <iostream>


class Melody
{
private:
    struct PositionedFigure : public Figure
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

#endif
