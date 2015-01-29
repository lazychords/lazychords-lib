#include <iostream>
#include <vector>
#include <map>
#include <stack>

#include <stdio.h>
#include <string>
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <sstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Weffc++"

#include <ELL/Grammar.h>
#include "ELL/Parser.h"

#pragma GCC diagnostic pop

#include "MusicRules.hpp"
#include "Utilities.hpp"

// ExprA Parser part
Number fractionToNumber(const Fraction& f) __attribute__((const));
Number fractionToNumber(const Fraction& f)
{
    return static_cast<Number>(f.numerator())/static_cast<Number>(f.denominator());
}
struct ExprAParser : ell::Parser<char>, ell::Grammar<char>
{
    ExprAParser();
    std::pair<ExprA, std::set<std::string> > parseExprA(const std::string& s)
    {
        parse(s.c_str());
        return std::make_pair(stack.top(), variables);
    }
    //static Logic::FormulaTree loadFormula(const std::string& filename, bool debug=false);
protected:
    std::stack<ExprA> stack;
    std::set<std::string> variables;
    template<typename T> void push(const T& f)
    {
        stack.push(ExprA(f));
    }
    ExprA pop()
    {
        ExprA res = stack.top();
        stack.pop();
        return res;
    }
    void getVariable(const ell::string & v)
    {
        std::string s;
        s+=v;
        if(s.find(' ')!=std::string::npos){
            throw std::runtime_error("There is a space in variable, strange...");
        }
        variables.insert(s);
        push([s](const std::map<std::string, Number>& m)
             {
                 if(m.find(s)!=m.end())
                     return m.find(s)->second;
                 else
                     throw std::runtime_error("Could not find variable " + s + " in map value");
             });
    }
    void abs()
    {
        ExprA top = pop();
        push([top](const std::map<std::string, Number>& m){return std::abs(top(m));});
    }
    void equals()
    {
        ExprA a = pop();
        ExprA b = pop();
        push([a,b](const std::map<std::string, Number>& m){return static_cast<int>(a(m))==static_cast<int>(b(m));});
    }
    void pushNumber(Number n)
    {
        push([n](const std::map<std::string, Number>&){return n;});
    }
    void pow()
    {
        ExprA a = pop();
        ExprA b = pop();
        push([a,b](const std::map<std::string, Number>& m){return std::pow(b(m),a(m));});
    }
    void mult()
    {
        ExprA a = pop();
        ExprA b = pop();
        push([a,b](const std::map<std::string, Number>& m){return b(m)*a(m);});
    }
    void divide()
    {
        ExprA a = pop();
        ExprA b = pop();
        push([a,b](const std::map<std::string, Number>& m){return b(m)/a(m);});
    }
    void add()
    {
        ExprA a = pop();
        ExprA b = pop();
        push([a,b](const std::map<std::string, Number>& m){return b(m)+a(m);});
    }
    void substract()
    {
        ExprA a = pop();
        ExprA b = pop();
        push([a,b](const std::map<std::string, Number>& m){return b(m)-a(m);});
    }
    void opposite()
    {
        ExprA top = pop();
        push([top](const std::map<std::string, Number>& m){return -top(m);});
    }
    ell::Rule<char> root, exprA, sum, factor, power, func, number, variable;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
ExprAParser::ExprAParser(): ell::Parser<char>(& root, & blank)
{
    flags.look_ahead = false;
    variable = no_skip((lower >> !(ch('('))>> *((ch(',') | alnum )) >> !ch(')')) [&ExprAParser::getVariable]);
    func = (istr("ABS(") >> exprA >> ch(')')) [&ExprAParser::abs]
        | (ch('[') >> exprA >> ch('=') >> exprA >> ch(']')) [&ExprAParser::equals];
    number = variable
        | real [&ExprAParser::pushNumber]
        | ch('(')>> exprA >> ch(')')
        | func;

    power = number >> (! (ch('^') >> power) [&ExprAParser::pow]);
    factor = power >>
        *( (ch('*') >> power) [&ExprAParser::mult]
           | (ch('/') >> power) [&ExprAParser::divide]);
    sum = factor >>
        *((ch('+') >> factor) [&ExprAParser::add]
          | (ch('-') >> factor) [&ExprAParser::substract]);
    exprA = sum
        | (ch('-') >> sum [&ExprAParser::opposite]);
    root = exprA >> ch(';');

    ELL_NAME_RULE(func);
    ELL_NAME_RULE(variable);
    ELL_NAME_RULE(number);
    ELL_NAME_RULE(power);
    ELL_NAME_RULE(factor);
    ELL_NAME_RULE(sum);
    ELL_NAME_RULE(exprA);
    ELL_NAME_RULE(root);
}
#pragma GCC diagnostic pop

ExprA getExprA(const std::string& chaine, const std::vector<std::string>& variables)
{
    if(chaine.find(';')!=std::string::npos){
        throw std::runtime_error("There should be no \";\" in string to parse");
    }
    std::string chtmp = chaine +';';
    ExprAParser parser;
    auto res = parser.parseExprA(chtmp);
    std::string error;
    for(auto it : res.second){
        if(std::find(variables.begin(), variables.end(), it)==variables.end()){
            error+=(it+ " ");
        }
    }
    if(error!=""){
        throw std::runtime_error("The following unknown variables are present in the arithmetic expression " +error);
    }
    return res.first;
}

ExprA getExprA(const std::string& chaine)
{
    if(chaine.find(';')!=std::string::npos){
        throw std::runtime_error("There should be no \";\" in string to parse");
    }
    std::string chtmp = chaine +';';
    ExprAParser parser;
    auto res = parser.parseExprA(chtmp);
    return res.first;
}


// Rules part

RuleCombination::RuleCombination(Rule1 r1, Rule2 r2, Rule3 r3, Rule4 r4, Rule5 r5, Rule6 r6, M1 m1, M2 m2, M3 m3, M4 m4) : m_r0(std::vector<Number>(Chord::nbChords, 0)), m_r1(r1), m_r2(r2), m_r3(r3), m_r4(r4), m_r5(r5), m_r6(r6), m_m1(m1), m_m2(m2), m_m3(m3), m_m4(m4)
{
}

RuleCombination getRules(const std::string& file, const std::vector<std::string>& ruleNames)
    // NB : some modifs in the end, just means for now
{
    //const std::vector<std::string> variables;

    auto nameSize = ruleNames.size();
    std::vector<bool> matched(nameSize,false); //matched[i] = the ith requested rule exists in file
    std::vector<bool> ruleType(6,false); //ruleType[i] = a rule of type i has been requested
    std::string line, ruleName, type, style, mode, data, sx, sExprA;
    std::pair<ExprA, Number> pairRule3;
    unsigned typeN; //ID of the rule
    double x3 = 0;
    std::vector<ExprA> rulesExpr(6);
    std::size_t pos, pos2, pos3, pos4; //positions of the punctuation
    std::ifstream workFile;
    workFile.open(file);

    if (workFile.is_open()){
        while(!workFile.eof()){
            std::getline (workFile,line);         //Bug ?
            pos = line.find(":"); //1er ";"
            assert(pos!=0 && pos+2!=std::string::npos); //format assertion
            ruleName = line.substr(0,pos);
            std::vector<std::string>::const_iterator it = find(ruleNames.begin(),ruleNames.end(),ruleName); // rule requested ; ruleNames.end() if it doesn't exist
            unsigned namePos = (unsigned)(it - ruleNames.begin()); //index corresponding to the element found
            if(it!=ruleNames.end() && matched[namePos] == false){ //the ith line requests the current line, and the rule hasn't been used yet
                matched[namePos]=true;
                pos2 = line.find(",", pos+1); //1er ","
                assert(pos2+2!=std::string::npos);
                type = line.substr(pos+2,pos2-pos-2);
                typeN = (unsigned)(type[0]-'0');       //ASSUMING FORMAT IS : "Name: i, truc,..."
                if(1<=typeN && typeN <= 6 && !ruleType[typeN]){
                    ruleType[typeN-1] = true;
                    pos3 = line.find(",", pos2+1); //2e ","
                    assert(pos3!=std::string::npos);
                    style = line.substr(pos2+2, pos3-pos2-2);
                    pos4 = line.find(",", pos3+1);
                    assert(pos4!=std::string::npos);
                    mode = line.substr(pos3+2,pos4-pos3-2);
                    data = line.substr(pos4+2);
                    if(typeN == 3){
                        pos=data.find(";");
                        if(pos!=std::string::npos){ //parameters

                            sx = data.substr(0,pos-1);      //Asuming format for data is : "parameter;...;ExprA"
                            //x3 = std::stod(sx); // ??
                            //istringstream::istringstream(sx);
                            std::istringstream parser(sx);
                            parser>>x3;
                            sExprA = data.substr(pos+1);
                        }
                        pairRule3.first = getExprA(sExprA);//, variables);
                        pairRule3.second = x3;
                    }else{
                        sExprA = data;
                    }
                    rulesExpr[typeN-1] = getExprA(sExprA);//, variables);
                }else{
                    throw std::runtime_error("The following rule is undefined or already used : " + typeN);//a rule is applied more than once!
                }
            }
        }

        for(unsigned i = 0; i<nameSize; i++){
            if (matched[i]==false){
                throw std::runtime_error("The following rule has not been found : " + ruleNames[i]);
            }
        }
        //TODO : FIX THIS !!!
        // WARNING : I CHANGED THIS IN ORDER TO COMPILE, NO MORE RULE6
        for(unsigned i = 0; i<5 /* io of 6, beware */; i++){
            if (ruleType[i]==false){
                throw std::runtime_error("No rule of type " + (i+1));
            }
        }
    }
    workFile.close();

    // Just to fix it temporaly, to test : doesn't take into acounts ExprA for means, and reference chords

    std::vector<std::pair<ChordProgression,Number> > cpVector;
    ExprA m1 = getExprA("r(1)+r(2)+r(3)+r(4)+r(5)");
    ExprA m2 = getExprA("m(1)+m(2)+m(3)+m(4)");
    ExprA m3 = getExprA("b+r(6)");
    ExprA m4 = getExprA("r*l");
    RuleCombination res(rulesExpr[0], rulesExpr[1], pairRule3, rulesExpr[3],rulesExpr[4], cpVector, m1, m2, m3, m4);
    return res;
}


// Rules done, modif have to be made

// Rules
std::map<std::string,Number> getAffectR1(const Chord& c, const Measure& m);
std::map<std::string,Number> getAffectR2(const Chord& c, const Measure& m);
std::map<std::string,Number> getAffectR3(const Chord& c, const Measure& m, Number x);
std::map<std::string,Number> getAffectR4(const Chord& c, const Key& k);
std::map<std::string,Number> getAffectR5(const Chord& c0, const Chord& c1);
Number getScoreR6(const ChordProgression& cp, const std::vector< std::pair<ChordProgression, Number> > &cpRef);

std::map<std::string,Number> getAffectR1(const Chord& c, const Measure& m)
{
    std::map<std::string,Number> affectation;
    std::vector<CompleteNoteName> notes = c.getNotes();
    for (unsigned i=0; i<4; i++){
        for (unsigned j=0; j<4; j++){
            std::string var = "v(" + toString(j+1) + ',' + toString(i+1) + ')';

            if (i < notes.size()){ //Check for the seventh
                Measure::ConstIterator note = m.getNoteBeingPlayedAt(Measure::Position(static_cast<int>(j)));
                if (CompleteNoteName(note->second.m_name,note->second.m_modifier) == notes[i]){
                    affectation.insert(std::pair<std::string,Number>(var,1));
                }else{
                    affectation.insert(std::pair<std::string,Number>(var,0));
                }
            }else{
                affectation.insert(std::pair<std::string,Number>(var,-1));
            }
        }
    }
    return affectation;
}

std::map<std::string,Number> getAffectR2(const Chord& c, const Measure& m)
{
    std::map<std::string,Number> affectation;
    std::vector<CompleteNoteName> notes = c.getNotes();
    Number v1 = 0;
    Number v2 = 0;
    Number v3 = 0;
    Number v4 = 0;
    for (Measure::ConstIterator note = m.begin(); note != m.end(); note++) {
        CompleteNoteName noteName = CompleteNoteName(note->second.m_name,note->second.m_modifier);
        if (noteName == notes[0]) {
            v1 += fractionToNumber(note->second.m_duration);
        } else if (noteName == notes[1]) {
            v2 +=fractionToNumber(note->second.m_duration);
        } else if (noteName == notes[2]) {
            v3 += fractionToNumber(note->second.m_duration);
        }
        else if (notes.size() > 3)
        {
            if(noteName == notes[3])
            {
                v4 += fractionToNumber(note->second.m_duration);
            }
        }
    }
    affectation.insert(std::pair<std::string,Number>("v(1)",v1/m.getDuration()));
    affectation.insert(std::pair<std::string,Number>("v(2)",v2/m.getDuration()));
    affectation.insert(std::pair<std::string,Number>("v(3)",v3/m.getDuration()));
    affectation.insert(std::pair<std::string,Number>("v(4)",v4/m.getDuration()));
    return affectation;
}

std::map<std::string,Number> getAffectR3(const Chord& c, const Measure& m, Number x)
{
    std::map<std::string,Number> affectation;
    std::vector<CompleteNoteName> notes = c.getNotes();
    Number v=0;
    for (Measure::ConstIterator note = m.begin(); note != m.end(); note++) {
        if (fractionToNumber(note->second.m_duration) > x) {
            CompleteNoteName noteName = CompleteNoteName(note->second.m_name,note->second.m_modifier);
            if (noteName != notes[0] && noteName != notes[1] && noteName != notes[2])
            {
                if(notes.size()<4)
                    v += fractionToNumber(note->second.m_duration);
                else
                {
                    if(noteName != notes[3])
                        v += fractionToNumber(note->second.m_duration);
                }
            }
        }
    }
    affectation.insert(std::pair<std::string,Number>("v",v/m.getDuration()));
    return affectation;
}

std::map<std::string,Number> getAffectR4(const Chord& c, const Key& k)
{
    std::map<std::string,Number> affectation;
    std::vector<CompleteNoteName> chord_notes = c.getNotes();
    std::vector<CompleteNoteName> key_notes = k.getNotes();
    for (unsigned i = 0; i<4; i++){
        std::string var = "v(" + toString(i+1) + ')';
        if (i < chord_notes.size() && find(key_notes.begin(),key_notes.end(), chord_notes[i]) != key_notes.end()){
            affectation.insert(std::pair<std::string,Number>(var,1));
        }else if(i<chord_notes.size() || chord_notes.size()==4)
        {
            affectation.insert(std::pair<std::string,Number>(var,0));
        }
        else
            affectation.insert(std::pair<std::string,Number>(var,-1));
    }
    return affectation;
}

std::map<std::string,Number> getAffectR5(const Chord& c1, const Chord& c2)
{
    std::map<std::string,Number> affectation;
    std::vector<CompleteNoteName> c1Notes = c1.getNotes();
    std::vector<CompleteNoteName> c2Notes = c2.getNotes();
    for (unsigned i = 0; i < 4; i++){
        std::string var0 = "v(0," + toString(i+1) + ')';
        if (i < c1Notes.size()){
            affectation.insert(std::pair<std::string,Number>(var0,(Number) Note::noteToHalfTone(c1Notes[i])));
        }else{
            //ISSUE , not zero here !!!
            affectation.insert(std::pair<std::string,Number>(var0,-1));
        }
    }
    for (unsigned i = 0; i < 4; i++){
        std::string var1 = "v(1," + toString(i+1) + ')';
        if (i < c2Notes.size()){
            affectation.insert(std::pair<std::string,Number>(var1,(Number) Note::noteToHalfTone(c2Notes[i])));
        }else{
            // ISSUE, no zero here !!!
            affectation.insert(std::pair<std::string,Number>(var1,-1));
        }
    }

    return affectation;
}

Number getScoreR6(const ChordProgression& cp, const std::vector<std::pair<ChordProgression, Number> >& cpRef)
{
    std::map<std::string,Number> affectation;
    for (unsigned i = 0; i < cpRef.size(); i++){
        if (cp == cpRef[i].first){
            return cpRef[i].second;
        }
    }

    return 0;
}


// ExprA to combine the rules
std::map<std::string,Number> getAffectM1(const std::vector<Number>& scoresM1);
std::map<std::string,Number> getAffectM2(const std::vector<Number>& scoresM2);
std::map<std::string,Number> getAffectM3(Number scoreB, Number scoreR6);
std::map<std::string,Number> getAffectM4(Number scoreMusicRules, Number scoreML);

std::map<std::string,Number> getAffectM1(const std::vector<Number>& scoresM1)
{
    std::map<std::string,Number> affectation;
    for (unsigned i = 0; i < scoresM1.size(); i++){
        std::string var = "r(" + toString(i+1) + ')';
        affectation.insert(std::pair<std::string,Number>(var, scoresM1[i]));
    }
    return affectation;
}

std::map<std::string,Number> getAffectM2(const std::vector<Number>& scoresM2)
{
    std::map<std::string,Number> affectation;
    for (unsigned i = 0; i < scoresM2.size(); i++){
        std::string var = "m(" + toString(i+1) + ')';
        affectation.insert(std::pair<std::string,Number>(var, scoresM2[i]));
    }
    return affectation;
}

std::map<std::string,Number> getAffectM3(Number scoreB, Number scoreR6)
{
    std::map<std::string,Number> affectation;
    affectation.insert(std::pair<std::string,Number>("b", scoreB));
    affectation.insert(std::pair<std::string,Number>("r(6)", scoreR6));
    return affectation;
}

std::map<std::string,Number> getAffectM4(Number scoreMusicRules, Number scoreML)
{
    std::map<std::string,Number> affectation;
    affectation.insert(std::pair<std::string,Number>("r", scoreMusicRules));
    affectation.insert(std::pair<std::string,Number>("l", scoreML));
    return affectation;
}

Number moyenne(std::vector<Number> tab) __attribute__((const));
Number moyenne(std::vector<Number> tab)
{
    Number score = 0;
    for (unsigned i = 0; i < tab.size(); i++){
        score += tab[i];
    }
    return score;
}

Number scoreMeasure (const RuleCombination& rules, const Key& k, const Measure& m, const Chord& cbefore, const Chord& c);
Number scoreMeasure (const RuleCombination& rules, const Key& k, const Measure& m, const Chord& cbefore, const Chord& c)
{
    std::vector<Number> scoreRules;
    scoreRules.push_back(rules.m_r1(getAffectR1(c,m)));
    scoreRules.push_back(rules.m_r2(getAffectR2(c,m)));
    scoreRules.push_back(rules.m_r3.first(getAffectR3(c,m,rules.m_r3.second)));
    scoreRules.push_back(rules.m_r4(getAffectR4(c,k)));
    scoreRules.push_back(rules.m_r5(getAffectR5(cbefore,c)));

    return rules.m_m1(getAffectM1(scoreRules));
}

Number scoreBloc (const RuleCombination& rules, const Key& k,
                  const Measure& m1, const Measure& m2, const Measure& m3, const Measure& m4,
                  const Chord& cbefore, const Chord& c1, const Chord& c2, const Chord& c3, const Chord& c4);
Number scoreBloc (const RuleCombination& rules, const Key& k,
                  const Measure& m1, const Measure& m2, const Measure& m3, const Measure& m4,
                  const Chord& cbefore, const Chord& c1, const Chord& c2, const Chord& c3, const Chord& c4)
{
    // First, M1 rules 1 to 5, for four measures

    std::vector<Number> scoreFourMeasures;
    scoreFourMeasures.push_back(scoreMeasure(rules,k,m1,cbefore,c1));
    scoreFourMeasures.push_back(scoreMeasure(rules,k,m2,c1,c2));
    scoreFourMeasures.push_back(scoreMeasure(rules,k,m3,c2,c3));
    scoreFourMeasures.push_back(scoreMeasure(rules,k,m4,c3,c4));

    Number scoreB = rules.m_m2(getAffectM2(scoreFourMeasures));

    // Now Rule 6

    ChordProgression blocTest;
    blocTest.push_back(c1);
    blocTest.push_back(c2);
    blocTest.push_back(c3);
    blocTest.push_back(c4);

    Number scoreR6 = getScoreR6(blocTest,rules.m_r6);

    return rules.m_m3(getAffectM3(scoreB,scoreR6));
}

Number scoreChordProgression(const RuleCombination& rules, const std::pair<ChordProgression, Number>& output, const Melody& m);
Number scoreChordProgression(const RuleCombination& rules, const std::pair<ChordProgression, Number>& output, const Melody& m)
{
    if (m.size() != output.first.size()){
        throw std::runtime_error ("The chord progression hasn't the same length as the melody");
    }
    if (m.size() % 4 != 0){
        throw std::runtime_error ("The melody length isn't a multiple of 4");
    }
    if (m.size() == 0){
        throw std::runtime_error ("The melody is empty");
    }

    Melody::ConstIterator iterMeasure = m.begin();
    ChordProgression::const_iterator iterChord = output.first.begin();
    Chord cbefore = output.first[0];
    std::vector<Number> scoresParBloc;
    while(iterMeasure!=m.end()){
        const Measure& m1 = *iterMeasure;
        iterMeasure++;
        const Measure& m2 = *iterMeasure;
        iterMeasure++;
        const Measure& m3 = *iterMeasure;
        iterMeasure++;
        const Measure& m4 = *iterMeasure;
        iterMeasure++;

        const Chord& c1 = *iterChord;
        iterChord++;
        const Chord& c2 = *iterChord;
        iterChord++;
        const Chord& c3 = *iterChord;
        iterChord++;
        const Chord& c4 = *iterChord;
        iterChord++;

        scoresParBloc.push_back(scoreBloc(rules,m.getKey(),m1,m2,m3,m4,cbefore,c1,c2,c3,c4));

        cbefore = c4;
    }

    Number scoreMusicRules = moyenne(scoresParBloc);
    Number scoreML = output.second;

    Number score = rules.m_m4(getAffectM4(scoreMusicRules, scoreML));

    return score;
}

ChordProgression getBestChordProgression(const RuleCombination& rules, const MachineLearningOutput& ml, const Melody& m, bool bis);
ChordProgression getBestChordProgression(const RuleCombination& rules, const MachineLearningOutput& ml, const Melody& m, bool bis)
{
    // Here, we assume that all ChordProgressions have the same length, which is the length of the melody (a multiple of 4)

    Number score_max = std::numeric_limits<Number>::lowest();
    Key key = m.getKey();
    /*if(key.m_mode==NoteName::C){
        std::cout<<"mode de C"<<std::endl;
    }
    for(const auto& mes : m){
        for(const auto& it2 : mes){
            if(it2.second.m_modifier==Accidental::Flat){
                std::cout<<"Flat"<<std::endl;
            }else{
                if(it2.second.m_modifier==Accidental::Sharp){
                    std::cout<<"Not sharp"<<std::endl;
                }else{
                    std::cout<<"Nada"<<std::endl;
                }
            }
        }
    }
	unsigned deca = (Note::noteToHalfTone(CompleteNoteName(key.m_mode,Accidental::None))-Note::noteToHalfTone(m.getKey().m_base)+12)%12;
    if(deca<100){
        std::cout<<deca<<std::endl;
    }*/

    Melody m2 = m.transpose(Key(CompleteNoteName(key.m_mode,Accidental::None),key.m_mode));
    ChordProgression sequence;
    if (bis)
    {
        std::vector<std::string> swanted =
	    {"C", "Dm", "Em", "F", "G", "Am",
	     "Dm7", "Em7", "Am7"};
        for(auto sc : swanted)
        {
            Chord tmp = readChord(sc);
            rules.m_r0[static_cast<unsigned>(tmp)] = 10000000;
	    }
	    for(auto& val : rules.m_r0)
            val-=10000000;
        sequence=scoreChordProgressionBis(rules, ml, m2);
    }
    else
    {
        for (auto output : ml)
        {
            Number score = scoreChordProgression(rules, output, m2);

            if (score > score_max){
                score_max = score;
                sequence = output.first;
            }
        }
	    std::cout << "Value of sequence : " << score_max << std::endl;
    }

    unsigned decalage = (Note::noteToHalfTone(key.m_base)-Note::noteToHalfTone(key.m_mode)+12)%12;
    return moveCP(sequence,static_cast<int>(decalage),key);
}

ChordProgression getBestChordProgression(const std::string& ruleFile, const std::vector<std::string>& ruleNames, const MachineLearningOutput& ml, const Melody& m, bool bis)
{
    RuleCombination rules = getRules(ruleFile, ruleNames);
    return getBestChordProgression(rules, ml, m, bis);
}

using ChordID = unsigned;

void Check(const std::map<std::string, Number>& a, const std::map<std::string, Number>& b, const Measure& m, const Chord& c);
void Check(const std::map<std::string, Number>& a, const std::map<std::string, Number>& b, const Measure& m, const Chord& c)
{
    unsigned idv=0;
    for(auto ita = a.begin(), itb = b.begin(); ita!=a.end();ita++, itb++, idv++)
    {
        std::cout<<"Old "<<ita->first<<" : "<<ita->second<<"\n";
        std::cout<<"New "<<itb->first<<" : "<<itb->second<<"\n";
        std::string tmp;
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wfloat-equal"
        if(ita->first!=itb->first || itb->second !=ita->second)
        {
            std::cout<<"idv : "<<idv<<"\n";
            std::cout<<"m : "<<m<<"\n";
            std::cout<<"c : "<<c<<"\n";
            std::cin>>tmp;

        }
        #pragma GCC diagnostic pop
        std::cout<<"\n";
    }
}

std::function<Number(ChordID)> evalFromMeasure(const Measure& m, const RuleCombination& rules, const  Melody& med);std::function<Number(ChordID)> evalFromMeasure(const Measure& m, const RuleCombination& rules, const  Melody& med)
{
    static std::map<std::string, Number> varR1 =
    {
        {"v(1,1)", 0}, {"v(1,2)", 0}, {"v(1,3)", 0}, {"v(1,4)", 0},
        {"v(2,1)", 0}, {"v(2,2)", 0}, {"v(2,3)", 0}, {"v(2,4)", 0},
        {"v(3,1)", 0}, {"v(3,2)", 0}, {"v(3,3)", 0}, {"v(3,4)", 0},
        {"v(4,1)", 0}, {"v(4,2)", 0}, {"v(4,3)", 0}, {"v(4,4)", 0}
    };
    static std::map<std::string, Number> varR2 = {{"v(1)", 0}, {"v(2)", 0}, {"v(3)", 0}, {"v(4)", 0}};
    static std::map<std::string, Number> varR3 = {{"v", 0}};
    static std::map<std::string, Number> varR4 = {{"v(1)", 0}, {"v(2)", 0}, {"v(3)", 0}, {"v(4)", 0}};
    static std::vector<Note> notes = std::vector<Note>(4);
    static std::vector<Number> apparitions = std::vector<Number>(12);
    static std::vector<Note> longuest;
    static std::vector<CompleteNoteName> keyNotes;
    keyNotes = med.getKey().getNotes();
    longuest.clear();
    //Comment traite les silences ?
    for(unsigned i=0;i<4;i++)
    {
        Note n;
        try
        {
            n = m.getNoteBeingPlayedAt(Measure::Position(static_cast<int>(i)))->second;
        }
        catch(const std::domain_error& e)
        {
            n = Note(NoteName::C, Accidental::None, 4, 0);
        }
        notes[i]=n;
    }

    for(auto& v : apparitions)
        v=0;
    for(auto& it : m)
    {
        apparitions[Note::noteToHalfTone(it.second)]+=fractionToNumber(it.second.m_duration)/m.getDuration();
        if(fractionToNumber(it.second.m_duration) >=rules.m_r3.second)
            longuest.push_back(it.second);
    }

    auto lambda = [&rules, &m, &med](ChordID c)
    {
        const std::vector<CompleteNoteName>& cnotes = Chord::getNotes(c);
        auto it = varR1.begin();
        unsigned i =0;
        while(it!=varR1.end())
        {
            if(notes[i/4].m_duration !=0)
            {
                if(i%4<3 || cnotes.size()==4)
                    it->second = static_cast<Number>(CompleteNoteName(notes[i/4].m_name,notes[i/4].m_modifier) == cnotes[i%4]);
                else
                    it->second = -1;
            }

            else
                it->second = 0;
            i++;
            it++;
        }

        it = varR2.begin();
        for(auto& cit : cnotes)
        {
            it->second = apparitions[Note::noteToHalfTone(cit)];
            it++;
        }
        if(cnotes.size()==3)
            it->second=0;

        varR3.begin() ->second=0;
        for(auto& n : longuest)
        {
            const CompleteNoteName& name = n;
            if(name != cnotes[0] && name != cnotes[1] && name != cnotes[2])
            {
                if(cnotes.size()==4)
                {
                    if(name !=cnotes[3])
                        varR3.begin()->second +=fractionToNumber(n.m_duration);
                }
                else
                    varR3.begin()->second +=fractionToNumber(n.m_duration);
            }
        }
        varR3.begin()->second/=m.getDuration();

        it = varR4.begin();
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunsafe-loop-optimizations"
        for(const auto& cit : cnotes)
        {
            it->second = static_cast<Number>(std::find(keyNotes.begin(), keyNotes.end(), cit) != keyNotes.end());
            it++;
        }
        #pragma GCC diagnostic pop
        if(cnotes.size()==3)
            it->second = -1;

        /*auto varR1old = getAffectR1(Chord(c), m);
        Check(varR1old, varR1, m, Chord(c));
        auto varR2old = getAffectR2(Chord(c), m);
        Check(varR2old, varR2, m, Chord(c));
        auto varR3old = getAffectR3(Chord(c), m, rules.m_r3.second);
        Check(varR3old, varR3, m, Chord(c));
        auto varR4old = getAffectR4(Chord(c), med.getKey());
        Check(varR4old, varR4, m, Chord(c));*/
        return rules.m_r0[c]+rules.m_r1(varR1)+rules.m_r2(varR2)+rules.m_r3.first(varR3)+rules.m_r4(varR4);
    };
    return lambda;
}

Number getR5(ChordID a, ChordID b, const Rule5& r);
Number getR5(ChordID a, ChordID b, const Rule5& r)
{
    static std::map<std::string, Number> varR5 =
    {
        {"v(0,1)", 0}, {"v(0,2)", 0}, {"v(0,3)", 0}, {"v(0,4)", 0},
        {"v(1,1)", 0}, {"v(1,2)", 0}, {"v(1,3)", 0}, {"v(1,4)", 0}
    };
    unsigned i=0;
    auto it = varR5.begin();
    for(auto& n : Chord::getNotes(a))
    {
        it->second = Note::noteToHalfTone(n);
        i++;
        it++;
    }
    while(i<4)
    {
        it->second = -1;
        i++;
        it++;
    }
    for(auto& n : Chord::getNotes(b))
    {
        it->second = Note::noteToHalfTone(n);
        i++;
        it++;
    }
    while(i<8)
    {
        it->second = -1;
        i++;
        it++;
    }
    return r(varR5);

}

ChordProgression scoreChordProgressionBis(const RuleCombination& rules, const MachineLearningOutput& ml, const Melody& melody)
{
    if(melody.size()==0)
        return ChordProgression();
    std::vector<std::pair<std::vector<ChordID> , Number> > last(Chord::nbChords);
    std::vector<Number> mlChordValues;

    auto it = melody.begin();
    {
        auto evalMeasure = [&](ChordID c)
        {
            //if(std::find(wantedChordsID.begin(), wantedChordsID.end(), c) != wantedChordsID.end())
                return evalFromMeasure(*it, rules, melody)(c);
            /*else
                return static_cast<Number>(-1000000);*/
        };
        for(auto& mlv : ml)
        {
            unsigned j = static_cast<unsigned>(mlv.first[0]);
            mlChordValues.push_back(mlv.second + evalMeasure(j) + getR5(j, j, rules.m_r5));
        }
        for(ChordID j=0;j<Chord::nbChords;j++)
            last[j] = std::make_pair(std::vector<ChordID>(1, j), evalMeasure(j) + getR5(j, j, rules.m_r5));
    }
    it++;
    unsigned currentMeasure =1;
    while(it!=melody.end())
    {
        auto evalMeasure = [&](ChordID c)
        {
            //if(std::find(wantedChordsID.begin(), wantedChordsID.end(), c) != wantedChordsID.end())
                return evalFromMeasure(*it, rules, melody)(c);
            /*else
                return static_cast<Number>(-1000000);*/
        };
        for(unsigned mlc =0;mlc<mlChordValues.size();mlc++)
        {
            unsigned j = static_cast<unsigned>(ml[mlc].first[currentMeasure]);
            unsigned k = static_cast<unsigned>(ml[mlc].first[currentMeasure-1]);
            mlChordValues[mlc]+=evalMeasure(j)+getR5(k,j,rules.m_r5);
        }
        std::vector<std::pair<std::vector<ChordID> , Number> > newlast(Chord::nbChords);
        for(ChordID j=0;j<Chord::nbChords;j++)
        {
            Number bestVal =std::numeric_limits<Number>::lowest();
            ChordID best;
            Number measureVal = evalMeasure(j);
            for(ChordID k=0;k<Chord::nbChords;k++)
            {
                Number val = last[k].second + getR5(k, j, rules.m_r5);
                if(val > bestVal)
                {
                    bestVal = val;
                    best = k;
                }
            }
            newlast[j] = std::make_pair(last[best].first, bestVal+measureVal);
            newlast[j].first.push_back(j);
        }
        last = newlast;
        it++;
        currentMeasure++;
    }
    Number best = std::numeric_limits<Number>::lowest();
    std::vector<ChordID> result;
    for(auto& p : last)
    {
        if(p.second > best)
        {
            best = p.second;
            result = p.first;
        }
    }
    ChordProgression res;
    for(auto id : result)
        res.push_back(Chord(id));
    //std::cout<<"Best value without ml : "<<best<<"\n";
    for(unsigned i=0;i<mlChordValues.size();i++)
    {
        if(mlChordValues[i] > best)
        {
            best = mlChordValues[i];
            res = ml[i].first;
        }
    }
    //std::cout<<"Best value with ml : "<<best<<"\n";
    return res;
}


