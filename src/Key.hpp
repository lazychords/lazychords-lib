/**
 * @file   Key.hpp
 * @author  Julien and Nicolas
 * @date   Wed Mar 18 00:33:48 2015
 *
 * @brief  Describe the Key structure
 *
 *
 */
#ifndef KEY_HPP_INCLUDED
#define KEY_HPP_INCLUDED

#include "Pitch.hpp"

struct Key
{
    Pitch base; ///< Base note of the key
    Pitch mode; ///< Mode of the key

    void check() const;
    void save(std::ostream& o) const;
    static Key load(std::istream& i);
    unsigned id() const;
    static Key fromId(unsigned hashValue);
    static Key randomInstance();
    static unsigned maxId();
    static Key fromStream(std::istream& i);

    explicit Key(const Pitch& base = Pitch::C, const Pitch& mode = Pitch::C);
    Key(const Key&) = default;
    Key(Key&&) = default;
    Key& operator=(const Key&) = default;
    ~Key() = default;

    bool operator==(const Key&) const;
    bool operator!=(const Key&) const;

    const std::vector<Pitch>& getNotes() const;
};

std::ostream& operator<<(std::ostream& o, const Key& k);
std::istream& operator>>(std::istream& i, Key& k);



#endif
