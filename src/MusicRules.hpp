#ifndef MUSICRULES_HPP_INCLUDED
#define MUSICRULES_HPP_INCLUDED
#include <functional>
#include <vector>
#include <map>

#include "Music.hpp"
#include "Utilities.hpp"

using ExprA = std::function<Number(const std::map<std::string, Number>&)>;

using Rule0 = std::vector<Number>;
using Rule1 = ExprA;
using Rule2 = ExprA;
using Rule3 = std::pair<ExprA, Number>;
using Rule4 = ExprA;
using Rule5 = ExprA;
using Rule6 = std::vector< std::pair<ChordProgression, Number> >;
using M1 = ExprA;
using M2 = ExprA;
using M3 = ExprA;
using M4 = ExprA;


struct RuleCombination
{
    RuleCombination(Rule1, Rule2, Rule3, Rule4, Rule5, Rule6, M1, M2, M3, M4);
    mutable Rule0 m_r0;
    Rule1 m_r1;
    Rule2 m_r2;
    Rule3 m_r3;
    Rule4 m_r4;
    Rule5 m_r5;
    Rule6 m_r6;
    M1 m_m1;
    M2 m_m2;
    M3 m_m3;
    M4 m_m4;
};

/**@brief Parses an extended arithmetical expression given a vector of variables
  *@param variables is the vector of variables in the arithmetical expression
  *@param toParse is the string to parse
  *@throw std::runtime_error for parsing errors
  *@return A function that will compute the arithmetical expression given a map giving the value of each variable
  *The grammar for arithmetical expressions is as follows :
    ExprA = Number | Variable | Function | ExprA ^ ExprA | ExprA * ExprA | ExprA / ExprA | ExprA + ExprA | ExprA - ExprA | -ExprA
    Number = real as describes in the ELL documentation
    Variable = lower (letter | digit | _ | ( | , | ) )*
    Function = ABS(ExprA) | [ExprA=ExprA]
  *The ExprA can throw an std::runtime_error
**/
ExprA getExprA(const std::string& toParse, const std::vector<std::string>& variables);

/**@brief Same as above except that it will consider that it ill auto-detect variable names
**/
ExprA getExprA(const std::string& toParse);

/**@brief Parses the file to find the description of the rules used
  *@param file is the name of the file from which to get the rules
  *@param ruleNames is the name of the rules to find
  *@throw std::runtime_error "Rule " + RuleName + "not found in file "+ filename
  *@throw std::runtime_error No rule of type x
  *@throw std::runtime_error Two rules of type x
  *@return The RuleCombination to use
  *The format of the file is a set of lines where each line is as follows :
    RuleName : Type;Style;Mode;Data
    Where Type = 1 | 2 | 3 | 4 | 5 | 6 indicates the rule's type
    Where Style = all | pop | ...
    Where Mode = all | A | B | ... | F | G
    Where Data = ExprA if Type = 1 or 2 or 5 or 6
               = ExprA | ExprA;x if Type = 3 (if no x, assume x is 0)
               = ExprA;x if Type = 4
**/
RuleCombination getRules(const std::string& file, const std::vector<std::string>& ruleNames);

/**@brief Gives the best ChordProgression given the music Rules and the result of the ML part
  *@param rules is the rules used
  *@param ml is the result from the ML part
  *@param m is the melody
  *@return The ChordProgression
  *Basicly, the function goes through every chordProgression given by ML
   and calculates the score of the chordProgression. It then returns the best chordProgression.
**/
ChordProgression getBestChordProgression(const RuleCombination& rules, const MachineLearningOutput& ml, const Melody& melody);
ChordProgression scoreChordProgressionBis(const RuleCombination& rules, const MachineLearningOutput& ml, const Melody& melody);

/**@brief Gives the best ChordProgression given the music Rules and the result of the ML part
  *@param ruleFile is the file containing the music rules
  *@param ruleNames is the vector containing the names of the rules chosen
  *@param ml is the result from the ML part
  *@param m is the melody
  *@return The ChordProgression
  *Basicly, the function goes through every chordProgression given by ML
   and calculates the score of the chordProgression. It then returns the best chordProgression.
**/
ChordProgression getBestChordProgression(const std::string& ruleFile, const std::vector<std::string>& ruleNames, const MachineLearningOutput& ml, const Melody& m, bool bis);



#endif
