// This file is part of Ell library.
//
// Ell library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Ell library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Ell library.  If not, see <http://www.gnu.org/licenses/>.

#ifndef INCLUDED_ELL_RULE_H
#define INCLUDED_ELL_RULE_H

#include "Node.h"

namespace ell
{
/// A rule allows to introduce recursive definitions in a grammar as
/// it breaks the expression templates chain.
/// This class may not be derived
template <typename Token>
struct Rule : public ConcreteNodeBase<Token, Rule<Token> >
{
    using ConcreteNodeBase<Token, Rule<Token> >::match;

    Rule()
        : top(0), must_delete(false)
    {
        // Default unique name to avoid infinite recursion in dump
        std::ostringstream oss;
        oss << 'r' << this;
        name = oss.str();
    }

    // Not virtual
    ~Rule()
    {
        if (! top)
        {
            std::string msg = "Rule `" + name + "` not used";
            throw std::runtime_error(msg.c_str());
        }
        else
        {
            free();
        }
    }

    void free()
    {
        bool md = must_delete;
        const Node<Token> * t = top;
        top = 0;
        must_delete = false;

        if (t && md)
        {
            delete t;
        }
    }

    template <typename N>
    void operator = (const N & n)
    {
        free();

        if (n.get_kind() == "rule")
        {
            top = & n;
        }
        else
        {
            top = new N(n);
            must_delete = true;
        }
    }

    const Rule & operator = (const Rule & r)
    {
        free();
        top = & r;
        return r;
    }

#       define ELL_NAME_RULE(rule) rule.set_name(#rule)

    Rule & set_name(const std::string & n)
    {
        name = n;
        return * this;
    }

    /// A transparent rule must not call itself recursively (even indirectly from another rule)
    /// else, the description will lead to stack a overflow behaviour.
    Rule & set_transparent()
    {
        name.clear();
        return * this;
    }

    bool match(Parser<Token> * parser, Storage<void> &) const
    {
        ELL_BEGIN_PARSE
        match = top->parse(parser);
        ELL_END_PARSE
    }

    std::string get_kind() const
    {
        return "rule";
    }
    std::string get_value() const
    {
        return name;
    }
    const Node<Token> * get_child_at(int index) const
    {
        if (index == 0)
            return top;
        return 0;
    }

    const Node<Token> * top;
    std::string name;
    bool must_delete;

private:
    Rule(const Rule & other);
};
}

#endif // INCLUDED_ELL_RULE_H
