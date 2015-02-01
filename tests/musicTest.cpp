#include "catch.hpp"
#include "Music.hpp"
#include <climits>
#include <exception>
#include <sstream>
#include <array>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
static const array<NoteName,7> availableNotes  {NoteName::A,NoteName::B,NoteName::C,NoteName::D,NoteName::E,NoteName::F,NoteName::G};
static const array<Accidental,7> availableAccidentals  {Accidental::None,Accidental::Flat,Accidental::Sharp };

static map<string,NoteName> correspNotes {{"A",NoteName::A},{"a",NoteName::A},{"B",NoteName::B},{"b",NoteName::B},{"C",NoteName::C},{"c",NoteName::C},{"D",NoteName::D},{"d",NoteName::D},{"E",NoteName::E},{"e",NoteName::E},{"F",NoteName::F},    {"f",NoteName::F}};

static vector<CompleteNoteName> noteOrder {CompleteNoteName(NoteName::C,Accidental::None), CompleteNoteName(NoteName::D,Accidental::Flat), CompleteNoteName(NoteName::D,Accidental::None), CompleteNoteName(NoteName::D,Accidental::Sharp), CompleteNoteName(NoteName::E,Accidental::None), CompleteNoteName(NoteName::F,Accidental::None), CompleteNoteName(NoteName::G,Accidental::Flat),CompleteNoteName(NoteName::G,Accidental::None), CompleteNoteName(NoteName::G,Accidental::Sharp),CompleteNoteName(NoteName::A,Accidental::None), CompleteNoteName(NoteName::B,Accidental::Flat),CompleteNoteName(NoteName::B,Accidental::None)};

map<string,Accidental> correspAccidentals={{"^",Accidental::Sharp},{"_",Accidental::Flat},{"",Accidental::None}};

#pragma GCC diagnostic pop
string strOfOctave(Octave o) __attribute__((const));

string strOfOctave(Octave o){
    if(o==Octave(4)){
        return "";
    }
    if(o<4){
        string str="";
        for(int i=0;i<4-static_cast<int>(o);i++){
            str+=",";
        }
        return str;
    }
    string str="";
    for(int i=0;i<static_cast<int>(o)-4;i++){
        str+="'";
    }
    return str;
}
int mod(int a,int m) __attribute__((const));
int mod(int a,int m){
    if(a>0){
        return a%m;
    }
    return mod(a+m,m);
}
TEST_CASE("Test of NoteName","[NoteName]")
{
    SECTION("<< operator"){
        stringstream t;
        SECTION("A"){
            NoteName n = NoteName::A;
            t<<n;
            REQUIRE(t.str()=="A");
        }
        SECTION("B"){
            NoteName n = NoteName::B;
            t<<n;
            REQUIRE(t.str()=="B");
        }
        SECTION("C"){
            NoteName n = NoteName::C;
            t<<n;
            REQUIRE(t.str()=="C");
        }
        SECTION("D"){
            NoteName n = NoteName::D;
            t<<n;
            REQUIRE(t.str()=="D");
        }
        SECTION("E"){
            NoteName n = NoteName::E;
            t<<n;
            REQUIRE(t.str()=="E");
        }
        SECTION("F"){
            NoteName n = NoteName::F;
            t<<n;
            REQUIRE(t.str()=="F");
        }
        SECTION("G"){
            NoteName n = NoteName::G;
            t<<n;
            REQUIRE(t.str()=="G");
        }
    }

    SECTION("Operator >>"){
        SECTION("Valid"){
            for(auto c : correspNotes){
                stringstream t;
                t << c.first;
                CAPTURE(c.first);
                CAPTURE(c.second);
                NoteName N;
                REQUIRE_NOTHROW(t>>N);
                CAPTURE(N);
                REQUIRE(!t.fail());
                REQUIRE(N==c.second);
                string str;
                t>>str;
                REQUIRE(str=="");
            }
        }
        SECTION("Several notes"){
            stringstream t;
            t.str(string());
            t<<"A c"<<endl<<"D";
            NoteName N1,N2,N3;
            REQUIRE_NOTHROW(t>>N1>>N2>>N3);
            REQUIRE(!t.fail());
            REQUIRE(N1==NoteName::A);
            REQUIRE(N2==NoteName::C);
            REQUIRE(N3==NoteName::D);
            string str;
            t>>str;
            REQUIRE(str=="");
        }
        SECTION("Fail test"){
            string str = "H",str2;
            stringstream ss;
            ss<<str;
            NoteName N;
            REQUIRE_NOTHROW(ss>>N);
            REQUIRE(ss.fail());
            ss.clear();
            ss.str(string());
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            ss<<str;
            REQUIRE_THROWS_AS(ss>>N,std::stringstream::failure);
            ss.str(string()); // empty string case
            REQUIRE_THROWS_AS(ss>>N,std::stringstream::failure);
        }
    }
    SECTION("Identity << >>"){
        NoteName M;
        for(auto c : correspNotes){
            stringstream t;
            REQUIRE_NOTHROW(t<<c.second);
            REQUIRE_NOTHROW(t>>M);
            REQUIRE(!t.fail());
            REQUIRE(c.second==M);
        }
    }
}

TEST_CASE("Test of Accidental","[Accidental]")
{

    SECTION("Operator <<"){
        for(auto& c : correspAccidentals){
            stringstream ss;
            REQUIRE_NOTHROW(ss << c.second);
            REQUIRE(!ss.fail());
            REQUIRE(ss.str()==c.first);
        }
    }
    SECTION("Operator >>"){
        for(auto& c : correspAccidentals){
            Accidental A;
            stringstream ss;
            ss << c.first;
            REQUIRE_NOTHROW(ss>>A);
            REQUIRE(!ss.fail());
            REQUIRE(A==c.second);
            string s;
            ss >> s;
            REQUIRE(s=="");
        }
    }
    SECTION("Identity << >>"){
        Accidental M;
        for(auto c : correspAccidentals){
            stringstream t;
            REQUIRE_NOTHROW(t<<c.second);
            REQUIRE_NOTHROW(t>>M);
            REQUIRE(!t.fail());
            REQUIRE(c.second==M);
            string s;
            t >> s;
            REQUIRE(s=="");
        }
    }
    SECTION("No accidental case"){
        stringstream ss;
        ss<< "J";
        Accidental A;
        REQUIRE_NOTHROW(ss >> A);
        REQUIRE(!ss.fail());
        string s;
        ss >> s;
        REQUIRE(s=="J");
        REQUIRE(A==Accidental::None);
    }
}
TEST_CASE("Test of CompleteNotename","[CompleteNotename]")
{
    SECTION("Construction of the object"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                CompleteNoteName CN(n,a);
                REQUIRE(CN.m_name==n);
                REQUIRE(CN.m_modifier==a);
            }
        }
    }
    SECTION("Operator =="){
        CompleteNoteName ref(NoteName::C);
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                ref.m_name = n;
                ref.m_modifier = a;
                CompleteNoteName rr(n,a);
                REQUIRE(ref==rr);
            }
        }
    }
    SECTION("Operator ==, semantic cases"){
        REQUIRE(CompleteNoteName(NoteName::C,Accidental::Sharp)==CompleteNoteName(NoteName::D,Accidental::Flat));
        REQUIRE(CompleteNoteName(NoteName::D,Accidental::Sharp)==CompleteNoteName(NoteName::E,Accidental::Flat));
        REQUIRE(CompleteNoteName(NoteName::E,Accidental::Sharp)==CompleteNoteName(NoteName::F,Accidental::None));
        REQUIRE(CompleteNoteName(NoteName::F,Accidental::Sharp)==CompleteNoteName(NoteName::G,Accidental::Flat));
        REQUIRE(CompleteNoteName(NoteName::G,Accidental::Sharp)==CompleteNoteName(NoteName::A,Accidental::Flat));
        REQUIRE(CompleteNoteName(NoteName::A,Accidental::Sharp)==CompleteNoteName(NoteName::B,Accidental::Flat));
        REQUIRE(CompleteNoteName(NoteName::B,Accidental::Sharp)==CompleteNoteName(NoteName::C,Accidental::None));
        REQUIRE(CompleteNoteName(NoteName::C,Accidental::Flat)==CompleteNoteName(NoteName::B,Accidental::None));
        REQUIRE(CompleteNoteName(NoteName::F,Accidental::Flat)==CompleteNoteName(NoteName::E,Accidental::None));
    }
    SECTION("Operator !="){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                CompleteNoteName ref(n,a);
                for(auto nn : availableNotes){
                    for(auto aa : availableAccidentals){
                        CompleteNoteName ref2(nn,aa);
                        if(nn!=n||a!=aa){
                            if(
                               (nn!=NoteName::C || aa!=Accidental::Sharp || n!=NoteName::D || a!=Accidental::Flat) &&
                               (n!=NoteName::C || a!=Accidental::Sharp || nn!=NoteName::D || aa!=Accidental::Flat) &&
                               (nn!=NoteName::D || aa!=Accidental::Sharp || n!=NoteName::E || a!=Accidental::Flat) &&
                               (n!=NoteName::D || a!=Accidental::Sharp || nn!=NoteName::E || aa!=Accidental::Flat) &&
                               (nn!=NoteName::E || aa!=Accidental::Sharp || n!=NoteName::F || a!=Accidental::None) &&
                               (n!=NoteName::E || a!=Accidental::Sharp || nn!=NoteName::F || aa!=Accidental::None) &&
                               (n!=NoteName::F || a!=Accidental::Sharp || nn!=NoteName::G || aa!=Accidental::Flat) &&
                               (nn!=NoteName::F || aa!=Accidental::Sharp || n!=NoteName::G || a!=Accidental::Flat) &&
                               (nn!=NoteName::G || aa!=Accidental::Sharp || n!=NoteName::A || a!=Accidental::Flat) &&
                               (n!=NoteName::G || a!=Accidental::Sharp || nn!=NoteName::A || aa!=Accidental::Flat) &&
                               (nn!=NoteName::A || aa!=Accidental::Sharp || n!=NoteName::B || a!=Accidental::Flat) &&
                               (n!=NoteName::A || a!=Accidental::Sharp || nn!=NoteName::B || aa!=Accidental::Flat) &&
                               (nn!=NoteName::B || aa!=Accidental::None || n!=NoteName::C || a!=Accidental::Flat) &&
                               (n!=NoteName::B || a!=Accidental::None || nn!=NoteName::C || aa!=Accidental::Flat) &&
                               (nn!=NoteName::E || aa!=Accidental::None || n!=NoteName::F || a!=Accidental::Flat) &&
                               (n!=NoteName::E || a!=Accidental::None || nn!=NoteName::F || aa!=Accidental::Flat) &&
                               (nn!=NoteName::B || aa!=Accidental::Sharp || n!=NoteName::C || a!=Accidental::None) &&
                               (n!=NoteName::B || a!=Accidental::Sharp || nn!=NoteName::C || aa!=Accidental::None) ){

                                REQUIRE(ref!=ref2);
                                REQUIRE(!(ref==ref2));

                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator <<"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                stringstream ss,ss2;
                CompleteNoteName cn(n,a);
                ss<<a<<n;
                ss2<<cn;
                REQUIRE(ss.str()==ss2.str());
            }
        }
    }
    SECTION("Operator >>"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                stringstream ss;
                CompleteNoteName cn;
                ss<<a<<n;
                REQUIRE_NOTHROW( ss>>cn);
                REQUIRE(!ss.fail());
                REQUIRE(cn==CompleteNoteName(n,a));
                string str;
                ss>>str;
                REQUIRE(str=="");
            }
        }
    }
    SECTION("Identity >> <<"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                stringstream ss;
                CompleteNoteName cn(n,a),cn2;
                ss<<cn;
                REQUIRE_NOTHROW(ss>>cn2);
                REQUIRE(!ss.fail());
                REQUIRE(cn==cn2);
                string str;
                ss>>str;
                REQUIRE(str=="");
            }
        }
    }
    stringstream ss;
    SECTION("Error handling"){
        SECTION("Wrong note name"){
            ss.clear();
            ss.str(string());
            string str="_K",str2;
            ss<<str;
            CompleteNoteName cn;
            REQUIRE_NOTHROW(ss>>cn);
            REQUIRE(ss.fail());
            ss.clear();
            /*ss>>str2;
            REQUIRE(str==str2); //the failure must not affect the content of the buffer (exception safe)*/
            ss.str(string());
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            ss<<str;
            REQUIRE(str==ss.str());
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
            /*ss.clear();
            ss>>str2;
            REQUIRE(str==str2);*/
        }
        SECTION("Wrong accidental"){
            ss.clear();
            ss.str(string());
            string str="|C",str2;
            ss<<str;
            CompleteNoteName cn;
            REQUIRE_NOTHROW(ss>>cn);
            REQUIRE(ss.fail());
            /*ss>>str2;
            REQUIRE(str==str2); //the failure must not affect the content of the buffer (exception safe)*/
            ss.clear();
            ss.str(string());
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            ss<<str;
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
            /*ss>>str2;
            REQUIRE(str==str2);*/
        }
        SECTION("Empty string"){
            ss.clear();
            ss.str(string());
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            CompleteNoteName cn;
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
        }
    }
}

TEST_CASE("Test of Pitch","[Pitch]")
{
    SECTION("Construction of the object"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                CompleteNoteName CN(n,a);
                for(unsigned i=0;i<11;i++){
                    Pitch P(CN,Octave(i));
                    REQUIRE(P.m_name==n);
                    REQUIRE(P.m_modifier==a);
                    REQUIRE(P.m_octave==Octave(i));
                    Pitch P2(n,a,Octave(i));
                    REQUIRE(P2.m_name==n);
                    REQUIRE(P2.m_modifier==a);
                    REQUIRE(P2.m_octave==Octave(i));
                }
            }
        }
    }
    SECTION("Operator =="){
        Pitch ref(NoteName::C);
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    ref.m_name = n;
                    ref.m_modifier = a;
                    ref.m_octave = Octave(i);
                    Pitch rr(n,a,Octave(i));
                    REQUIRE(ref==rr);
                }
            }
        }
    }
    SECTION("Operator ==, semantic cases"){
        REQUIRE(Pitch(NoteName::C,Accidental::Sharp)==Pitch(NoteName::D,Accidental::Flat));
        REQUIRE(Pitch(NoteName::D,Accidental::Sharp)==Pitch(NoteName::E,Accidental::Flat));
        REQUIRE(Pitch(NoteName::E,Accidental::Sharp)==Pitch(NoteName::F,Accidental::None));
        REQUIRE(Pitch(NoteName::F,Accidental::Sharp)==Pitch(NoteName::G,Accidental::Flat));
        REQUIRE(Pitch(NoteName::G,Accidental::Sharp)==Pitch(NoteName::A,Accidental::Flat));
        REQUIRE(Pitch(NoteName::A,Accidental::Sharp)==Pitch(NoteName::B,Accidental::Flat));
        REQUIRE(Pitch(NoteName::B,Accidental::Sharp)==Pitch(NoteName::C,Accidental::None));
        REQUIRE(Pitch(NoteName::C,Accidental::Flat)==Pitch(NoteName::B,Accidental::None));
        REQUIRE(Pitch(NoteName::F,Accidental::Flat)==Pitch(NoteName::E,Accidental::None));
    }
    SECTION("Operator !="){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    Pitch ref(n,a,Octave(i));
                    for(auto nn : availableNotes){
                        for(auto aa : availableAccidentals){
                            for(unsigned j=0;j<11;j++){
                                Pitch ref2(nn,aa,Octave(j));
                                if(nn!=n||a!=aa||i!=j){
                                    if(
                                       (nn!=NoteName::C || aa!=Accidental::Sharp || n!=NoteName::D || a!=Accidental::Flat) &&
                                       (n!=NoteName::C || a!=Accidental::Sharp || nn!=NoteName::D || aa!=Accidental::Flat) &&
                                       (nn!=NoteName::D || aa!=Accidental::Sharp || n!=NoteName::E || a!=Accidental::Flat) &&
                                       (n!=NoteName::D || a!=Accidental::Sharp || nn!=NoteName::E || aa!=Accidental::Flat) &&
                                       (nn!=NoteName::E || aa!=Accidental::Sharp || n!=NoteName::F || a!=Accidental::None) &&
                                       (n!=NoteName::E || a!=Accidental::Sharp || nn!=NoteName::F || aa!=Accidental::None) &&
                                       (n!=NoteName::F || a!=Accidental::Sharp || nn!=NoteName::G || aa!=Accidental::Flat) &&
                                       (nn!=NoteName::F || aa!=Accidental::Sharp || n!=NoteName::G || a!=Accidental::Flat) &&
                                       (nn!=NoteName::G || aa!=Accidental::Sharp || n!=NoteName::A || a!=Accidental::Flat) &&
                                       (n!=NoteName::G || a!=Accidental::Sharp || nn!=NoteName::A || aa!=Accidental::Flat) &&
                                       (nn!=NoteName::A || aa!=Accidental::Sharp || n!=NoteName::B || a!=Accidental::Flat) &&
                                       (n!=NoteName::A || a!=Accidental::Sharp || nn!=NoteName::B || aa!=Accidental::Flat) &&
                                       (nn!=NoteName::B || aa!=Accidental::None || n!=NoteName::C || a!=Accidental::Flat) &&
                                       (n!=NoteName::B || a!=Accidental::None || nn!=NoteName::C || aa!=Accidental::Flat) &&
                                       (nn!=NoteName::E || aa!=Accidental::None || n!=NoteName::F || a!=Accidental::Flat) &&
                                       (n!=NoteName::E || a!=Accidental::None || nn!=NoteName::F || aa!=Accidental::Flat) &&
                                       (nn!=NoteName::B || aa!=Accidental::Sharp || n!=NoteName::C || a!=Accidental::None) &&
                                       (n!=NoteName::B || a!=Accidental::Sharp || nn!=NoteName::C || aa!=Accidental::None) ){
                                        REQUIRE(ref!=ref2);
                                        REQUIRE(!(ref==ref2));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator +="){
        SECTION("Postitive offset"){
            SECTION("Correct note name"){
                for(unsigned o=0;o<5;o++){
                    for(int i=0;i<50;i++){
                        for(unsigned j=0;j<noteOrder.size();j++){
                            Pitch P(noteOrder[j],Octave(o));
                            P+=i;
                            REQUIRE(P==Pitch(noteOrder[(j+static_cast<unsigned>(i))%noteOrder.size()],P.m_octave));
                        }
                    }
                }
            }
            SECTION("Octave switching"){
                for(unsigned k=0;k<=3;k++){
                    for(unsigned o=0;o<5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P(noteOrder[i],Octave(o));
                                P+=((int)noteOrder.size()-(int)i) + (int)j + (int)k*((int)noteOrder.size());
                                REQUIRE(P.m_octave==Octave(o)+1+k);
                            }
                        }
                    }
                }
            }
        }
        SECTION("Negative offset"){
            SECTION("Underflow handling"){
                for(unsigned k=0;k<=4;k++){
                    for(unsigned o=0;o<=5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P(noteOrder[i],Octave(o));
                                REQUIRE_THROWS_AS(P+= (-int((1+i) + j + (o+k)*(noteOrder.size()))),std::domain_error);
                                REQUIRE(P==Pitch(noteOrder[i],Octave(o))); //Exception safety
                                REQUIRE_THROWS_AS(P+= (-(INT_MAX)),std::domain_error);
                                REQUIRE(P==Pitch(noteOrder[i],Octave(o))); //Exception safety
                            }
                        }
                    }
                }
            }
            SECTION("Correct note name"){
                for(unsigned o=0;o<5;o++){
                    for(unsigned i=0;i<50;i++){
                        for(unsigned j=0;j<noteOrder.size();j++){
                            Pitch P(noteOrder[j],Octave(o));
                            if(i > o*noteOrder.size() + j){
                                REQUIRE_THROWS_AS(P+=(-int(i)),std::domain_error);
                            }else{
                                REQUIRE_NOTHROW(P+=(-int(i)));
                                int index = static_cast<int>(j);
                                index-=static_cast<int>(i);
                                index = mod(index,(int)noteOrder.size());
                                REQUIRE(P==Pitch(noteOrder[static_cast<unsigned>(index)],P.m_octave));
                            }
                        }
                    }
                }
            }
            SECTION("Octave switching"){
                for(unsigned k=0;k<=3;k++){
                    for(unsigned o=0;o<5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P(noteOrder[i],Octave(o));
                                if(i + j + 1 + k*(noteOrder.size()) > o*noteOrder.size() + i){
                                    REQUIRE_THROWS_AS(P+= -int(i + j + 1 + k*(noteOrder.size())),std::domain_error);
                                }else{
                                    REQUIRE_NOTHROW(P+=(-int(i + j + 1 + k*(noteOrder.size()))));
                                    REQUIRE(P.m_octave==(int)Octave(o)-(int)(1+k));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator +"){
        SECTION("[+]Postitive offset"){
            SECTION("[+]Correct note name"){
                for(unsigned o=0;o<5;o++){
                    for(int i=0;i<50;i++){
                        for(unsigned j=0;j<noteOrder.size();j++){
                            Pitch P,PP(noteOrder[j],Octave(o));
                            REQUIRE_NOTHROW( P=PP+i);
                            REQUIRE(P==Pitch(noteOrder[(j+(unsigned)i)%noteOrder.size()],P.m_octave));
                            REQUIRE(PP==Pitch(noteOrder[j],Octave(o)));
                        }
                    }
                }
            }
            SECTION("[+]Octave switching"){
                for(unsigned k=0;k<=3;k++){
                    for(unsigned o=0;o<5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P,PP(noteOrder[i],Octave(o));
                                P=PP+((int)noteOrder.size()-(int)i) + (int)j + (int)k*((int)noteOrder.size());
                                REQUIRE(P.m_octave==Octave(o)+1+k);
                                REQUIRE(PP==Pitch(noteOrder[i],Octave(o)));
                            }
                        }
                    }
                }
            }
        }
        SECTION("[+]Negative offset"){
            SECTION("[+]Underflow handling"){
                for(unsigned k=0;k<=4;k++){
                    for(unsigned o=0;o<=5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P(NoteName::E,Accidental::Flat,Octave(5)),PP(noteOrder[i],Octave(o));
                                REQUIRE_THROWS_AS(P= PP+(-int((1+i) + j + (o+k)*(noteOrder.size()))),std::domain_error);
                                REQUIRE(PP==Pitch(noteOrder[i],Octave(o)));  //no side effect
                                REQUIRE(P==Pitch(NoteName::E,Accidental::Flat,Octave(5))); //exception safety
                                REQUIRE_THROWS_AS(P=PP + ( -(INT_MAX)),std::domain_error);
                                REQUIRE(PP==Pitch(noteOrder[i],Octave(o)));  //no side effect
                                REQUIRE(P==Pitch(NoteName::E,Accidental::Flat,Octave(5))); //exception safety

                            }
                        }
                    }
                }
            }
            SECTION("[+]Correct note name"){
                for(unsigned o=0;o<5;o++){
                    for(unsigned i=0;i<50;i++){
                        for(unsigned j=0;j<noteOrder.size();j++){
                            Pitch P,PP(noteOrder[j],Octave(o));
                            if(i > o*noteOrder.size() + j){
                                REQUIRE_THROWS_AS(P=PP+(-(int)i),std::domain_error);
                            }else{
                                REQUIRE_NOTHROW(P=PP+(-(int)i));
                                int index = static_cast<int>(j);
                                index-=static_cast<int>(i);
                                index = mod(index,(int)noteOrder.size());
                                REQUIRE(P==Pitch(noteOrder[static_cast<unsigned>(index)],P.m_octave));
                                REQUIRE(PP==Pitch(noteOrder[j],Octave(o)));  //no side effect
                            }
                        }
                    }
                }
            }
            SECTION("[+]Octave switching"){
                for(unsigned k=0;k<=3;k++){
                    for(unsigned o=0;o<5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P,PP(noteOrder[i],Octave(o));
                                if(1+i + j + k*(noteOrder.size()) > o*noteOrder.size() + i){
                                    REQUIRE_THROWS_AS(P=PP+( -int(1+i + j + k*(noteOrder.size()))),std::domain_error);
                                }else{
                                    REQUIRE_NOTHROW(P=PP+(-int(i + 1 + j + k*(noteOrder.size()))));
                                    REQUIRE(P.m_octave==(int)Octave(o)-(int)(1+k));
                                    REQUIRE(PP==Pitch(noteOrder[i],Octave(o)));  //no side effect
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator -="){
        SECTION("[-=]Postitive offset"){
            SECTION("[-=]Correct note name"){
                for(unsigned o=0;o<5;o++){
                    for(int i=0;i<50;i++){
                        for(unsigned j=0;j<noteOrder.size();j++){
                            Pitch P(noteOrder[j],Octave(o));
                            P-=(-int(i));
                            REQUIRE(P==Pitch(noteOrder[(j+static_cast<unsigned>(i))%noteOrder.size()],P.m_octave));
                        }
                    }
                }
            }
            SECTION("[-=]Octave switching"){
                for(unsigned k=0;k<=3;k++){
                    for(unsigned o=0;o<5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P(noteOrder[i],Octave(o));
                                P-=(-int(((int)noteOrder.size()-(int)i) + (int)j + (int)k*((int)noteOrder.size())));
                                REQUIRE(P.m_octave==Octave(o)+1+k);
                            }
                        }
                    }
                }
            }
        }
        SECTION("[-=]Negative offset"){
            SECTION("[-=]Underflow handling"){
                for(unsigned k=0;k<=4;k++){
                    for(unsigned o=0;o<=5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P(noteOrder[i],Octave(o));
                                REQUIRE_THROWS_AS(P-=int((1+i) + j + (o+k)*(noteOrder.size())),std::domain_error);
                                REQUIRE(P==Pitch(noteOrder[i],Octave(o))); //Exception safety
                                REQUIRE_THROWS_AS(P-= (INT_MAX),std::domain_error);
                                REQUIRE(P==Pitch(noteOrder[i],Octave(o))); //Exception safety
                            }
                        }
                    }
                }
            }
            SECTION("[-=]Correct note name"){
                for(unsigned o=0;o<5;o++){
                    for(unsigned i=0;i<50;i++){
                        for(unsigned j=0;j<noteOrder.size();j++){
                            Pitch P(noteOrder[j],Octave(o));
                            if(i > o*noteOrder.size() + j){
                                REQUIRE_THROWS_AS(P-=int(i),std::domain_error);
                            }else{
                                REQUIRE_NOTHROW(P-=int(i));
                                int index = static_cast<int>(j);
                                index-= static_cast<int>(i);
                                index = mod(index,(int)noteOrder.size());
                                REQUIRE(P==Pitch(noteOrder[static_cast<unsigned>(index)],P.m_octave));
                            }
                        }
                    }
                }
            }
            SECTION("[-=]Octave switching"){
                for(unsigned k=0;k<=3;k++){
                    for(unsigned o=0;o<5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P(noteOrder[i],Octave(o));
                                if(1 + i + j + k*(noteOrder.size()) > o*noteOrder.size() + i){
                                    REQUIRE_THROWS_AS(P-=int( i + 1 + j + k*(noteOrder.size())),std::domain_error);
                                }else{
                                    REQUIRE_NOTHROW(P-= int(i + 1 + j + k*(noteOrder.size())));
                                    REQUIRE(P.m_octave==(int)Octave(o)-(int)(1+k));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator -"){
        SECTION("[-]Postitive offset"){
            SECTION("[-]Correct note name"){
                for(unsigned o=0;o<5;o++){
                    for(int i=0;i<50;i++){
                        for(unsigned j=0;j<noteOrder.size();j++){
                            Pitch P,PP(noteOrder[j],Octave(o));
                            REQUIRE_NOTHROW( P=PP-(-(int)i));
                            REQUIRE(P==Pitch(noteOrder[(j+static_cast<unsigned>(i))%noteOrder.size()],P.m_octave));
                            REQUIRE(PP==Pitch(noteOrder[j],Octave(o)));
                        }
                    }
                }
            }
            SECTION("[-]Octave switching"){
                for(unsigned k=0;k<=3;k++){
                    for(unsigned o=0;o<5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P,PP(noteOrder[i],Octave(o));
                                P=PP-(-int((noteOrder.size()-i) + j + k*(noteOrder.size())));
                                REQUIRE(P.m_octave==Octave(o)+1+k);
                                REQUIRE(PP==Pitch(noteOrder[i],Octave(o)));
                            }
                        }
                    }
                }
            }
        }
        SECTION("[-]Negative offset"){
            SECTION("[-]Underflow handling"){
                for(unsigned k=0;k<=4;k++){
                    for(unsigned o=0;o<=5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P(NoteName::E,Accidental::Flat,Octave(5)),PP(noteOrder[i],Octave(o));
                                REQUIRE_THROWS_AS(P= PP-int((1+i) + j + (o+k)*(noteOrder.size())),std::domain_error);
                                REQUIRE(PP==Pitch(noteOrder[i],Octave(o)));  //no side effect
                                REQUIRE(P==Pitch(NoteName::E,Accidental::Flat,Octave(5))); //exception safety
                                REQUIRE_THROWS_AS(P=PP -(INT_MAX),std::domain_error);
                                REQUIRE(PP==Pitch(noteOrder[i],Octave(o)));  //no side effect
                                REQUIRE(P==Pitch(NoteName::E,Accidental::Flat,Octave(5))); //exception safety

                            }
                        }
                    }
                }
            }
            SECTION("[-]Correct note name"){
                for(unsigned o=0;o<5;o++){
                    for(unsigned i=0;i<50;i++){
                        for(unsigned j=0;j<noteOrder.size();j++){
                            Pitch P,PP(noteOrder[j],Octave(o));
                            if(i > o*noteOrder.size() + j){
                                REQUIRE_THROWS_AS(P=PP-(int)i,std::domain_error);
                            }else{
                                REQUIRE_NOTHROW(P=PP-(int)i);
                                int index = static_cast<int>(j);
                                index-=static_cast<int>(i);
                                index = mod(index,(int)noteOrder.size());
                                REQUIRE(P==Pitch(noteOrder[static_cast<unsigned>(index)],P.m_octave));
                                REQUIRE(PP==Pitch(noteOrder[j],Octave(o)));  //no side effect
                            }
                        }
                    }
                }
            }
            SECTION("[-]Octave switching"){
                for(unsigned k=0;k<=3;k++){
                    for(unsigned o=0;o<5;o++){
                        for(unsigned i=0;i<noteOrder.size();i++){
                            for(unsigned j=0;j<noteOrder.size();j++){
                                Pitch P,PP(noteOrder[i],Octave(o));
                                if(i + 1 + j + k*(noteOrder.size()) > o*noteOrder.size() + i){
                                    REQUIRE_THROWS_AS(P=PP -int(i + 1 + j + k*(noteOrder.size())),std::domain_error);
                                }else{
                                    REQUIRE_NOTHROW(P=PP-int(i + 1 + j + k*(noteOrder.size())));
                                    REQUIRE(P.m_octave==Octave(o)-(1+k));
                                    REQUIRE(PP==Pitch(noteOrder[i],Octave(o)));  //no side effect
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    SECTION("Operator <<"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    stringstream ss,ss2;
                    CAPTURE(i)
                    Octave o(i>4 ? i-1 : i);
                    CAPTURE(o)
                    Pitch cn(n,a,Octave(i));
                    ss<<a<<n<<strOfOctave(o);
                    string target = ss.str();
                    if(i>4){
                        std::transform(target.begin(),target.end(),target.begin(),::tolower);
                    }
                    ss2<<cn;
                    REQUIRE(target==ss2.str());
                }
            }
        }
    }
    SECTION("Operator >>"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    stringstream ss;
                    Pitch cn;
                    ss<<a<<n<<strOfOctave(Octave(i));
                    REQUIRE_NOTHROW( ss>>cn);
                    REQUIRE(!ss.fail());
                    REQUIRE(cn==Pitch(n,a,Octave(i)));
                    string str;
                    ss>>str;
                    REQUIRE(str=="");
                }
            }
        }
    }
    SECTION("Identity >> <<"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    stringstream ss;
                    Pitch cn(n,a),cn2;
                    ss<<cn;
                    REQUIRE_NOTHROW(ss>>cn2);
                    REQUIRE(!ss.fail());
                    REQUIRE(cn==cn2);
                    string str;
                    ss>>str;
                    REQUIRE(str=="");
                }
            }
        }
    }
    stringstream ss;
    SECTION("Error handling"){
        SECTION("Wrong note name"){
            ss.clear();
            ss.str(string());
            string str="K~,,",str2;
            ss<<str;
            Pitch cn;
            REQUIRE_NOTHROW(ss>>cn);
            REQUIRE(ss.fail());
            ss.clear();
            ss>>str2;
            REQUIRE(str==str2);
            ss.clear();
            ss.str(string());
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            ss<<str;
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
            ss.clear();
            ss>>str2;
            REQUIRE(str==str2);
        }
        SECTION("Wrong accidental"){
            ss.clear();
            ss.str(string());
            string str="#E'''",str2;
            ss<<str;
            Pitch cn;
            REQUIRE_NOTHROW(ss>>cn);
            REQUIRE(ss.fail());
            ss.clear();
            ss>>str2;
            REQUIRE(str==str2);
            ss.clear();
            ss.str(string());
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            ss<<str;
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
            ss.clear();
            ss>>str2;
            REQUIRE(str==str2);
        }
        SECTION("Underflow"){
            ss.clear();
            ss.str(string());
            for(auto n : availableNotes){
                for(auto a : availableAccidentals){
                    string s=",,,,",over="";
                    for(unsigned i=5;i<11;i++){
                        over+=",";
                        ss.clear();
                        ss.str(string());
                        ss<<a<<n<<s<<over;
                        //string str=ss.str();
                        Pitch cn;
                        REQUIRE_NOTHROW(ss>>cn);
                        REQUIRE(!ss.fail());
                        string str;
                        ss>>str;
                        REQUIRE(over==str);
                        REQUIRE(cn==Pitch(n,a,Octave(0)));
                    }
                }
            }
        }
        SECTION("Empty string"){
            ss.clear();
            ss.str(string()); //empty string case
            Pitch cn;
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
        }
    }
}
TEST_CASE("Some parsing tests","[Parsing]")
{
    SECTION("Natural note"){
        stringstream ss;
        ss<<"bb";
        Accidental A;
        NoteName N;
        ss >> A >> N;
        string s;
        ss >> s;
        REQUIRE(s=="b");
        REQUIRE(A==Accidental::None);
        REQUIRE(N==NoteName::B);
    }
    SECTION("Several Notes"){
        stringstream ss;
        ss<<"E_E";
        CompleteNoteName cn,cn2;
        REQUIRE_NOTHROW(ss>>cn>>cn2);
        REQUIRE(!ss.fail());
        string s;
        ss >> s;
        REQUIRE(s=="");
        REQUIRE(cn.m_modifier==Accidental::None);
        REQUIRE(cn.m_name==NoteName::E);
        REQUIRE(cn2.m_modifier==Accidental::Flat);
        REQUIRE(cn.m_name==NoteName::E);
    }
}
#ifdef BOOST
TEST_CASE("Test of Note","[Note]")
{
    SECTION("Construction of the object"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    Pitch P(n,a,Octave(i));
                    for(unsigned j = 1;j<6;j++){
                        for(unsigned k = 1;k<6;k++){
                            Note N(P,Fraction((int)j,(int)k));
                            REQUIRE(N.m_name==n);
                            REQUIRE(N.m_modifier==a);
                            REQUIRE(N.m_octave==Octave(i));
                            REQUIRE(N.m_duration==Fraction((int)j,(int)k));
                            Note N2(n,a,Octave(i),Fraction((int)j,(int)k));
                            REQUIRE(N2.m_name==n);
                            REQUIRE(N2.m_modifier==a);
                            REQUIRE(N2.m_octave==Octave(i));
                            REQUIRE(N2.m_duration==Fraction((int)j,(int)k));
                        }
                    }
                }
            }
        }
    }
    SECTION("Construction of the object, failing cases"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    Pitch P(n,a,Octave(i));
                    for(unsigned j = 1;j<6;j++){
                        for(unsigned k = 1;k<6;k++){
                            REQUIRE_THROWS_AS(Note N(P,Fraction(-(int)j,(int)k)),std::domain_error);
                            REQUIRE_THROWS_AS(Note N2(n,a,Octave(i),Fraction((int)j,-(int)k)),std::domain_error);
                            REQUIRE_THROWS_AS(Note N(P,Fraction(0,(int)k)),std::domain_error);
                            REQUIRE_THROWS_AS(Note N2(n,a,Octave(i),Fraction(0,(int)k)),std::domain_error);
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator =="){
        Note ref(NoteName::C);
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    for(unsigned j = 1;j<6;j++){
                        for(unsigned k = 1;k<6;k++){
                            ref.m_name = n;
                            ref.m_modifier = a;
                            ref.m_octave = Octave(i);
                            ref.m_duration = Fraction((int)j,(int)k);
                            Note rr(n,a,Octave(i),Fraction((int)j,(int)k));
                            REQUIRE(ref==rr);
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator ==, semantic cases"){
        REQUIRE(Note(NoteName::C,Accidental::Sharp)==Note(NoteName::D,Accidental::Flat));
        REQUIRE(Note(NoteName::D,Accidental::Sharp)==Note(NoteName::E,Accidental::Flat));
        REQUIRE(Note(NoteName::E,Accidental::Sharp)==Note(NoteName::F,Accidental::None));
        REQUIRE(Note(NoteName::F,Accidental::Sharp)==Note(NoteName::G,Accidental::Flat));
        REQUIRE(Note(NoteName::G,Accidental::Sharp)==Note(NoteName::A,Accidental::Flat));
        REQUIRE(Note(NoteName::A,Accidental::Sharp)==Note(NoteName::B,Accidental::Flat));
        REQUIRE(Note(NoteName::B,Accidental::Sharp)==Note(NoteName::C,Accidental::None));
        REQUIRE(Note(NoteName::C,Accidental::Flat)==Note(NoteName::B,Accidental::None));
        REQUIRE(Note(NoteName::F,Accidental::Flat)==Note(NoteName::E,Accidental::None));
    }
    SECTION("Operator !="){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<3;i++){
                    for(unsigned j = 1;j<3;j++){
                        for(unsigned k = 1;k<3;k++){
                            Note ref(n,a,Octave(i),Fraction((int)j,(int)k));
                            for(auto nn : availableNotes){
                                for(auto aa : availableAccidentals){
                                    for(unsigned ii=0;ii<3;ii++){
                                        for(unsigned jj = 1;jj<3;jj++){
                                            for(unsigned kk = 1;kk<3;kk++){
                                                Note ref2(nn,aa,Octave(ii),Fraction((int)jj,(int)kk));
                                                if(nn!=n||a!=aa||i!=ii||Fraction((int)j,(int)k)!=Fraction((int)jj,(int)kk)){
                                                    if(
                                                       (nn!=NoteName::C || aa!=Accidental::Sharp || n!=NoteName::D || a!=Accidental::Flat) &&
                                                       (n!=NoteName::C || a!=Accidental::Sharp || nn!=NoteName::D || aa!=Accidental::Flat) &&
                                                       (nn!=NoteName::D || aa!=Accidental::Sharp || n!=NoteName::E || a!=Accidental::Flat) &&
                                                       (n!=NoteName::D || a!=Accidental::Sharp || nn!=NoteName::E || aa!=Accidental::Flat) &&
                                                       (nn!=NoteName::E || aa!=Accidental::Sharp || n!=NoteName::F || a!=Accidental::None) &&
                                                       (n!=NoteName::E || a!=Accidental::Sharp || nn!=NoteName::F || aa!=Accidental::None) &&
                                                       (n!=NoteName::F || a!=Accidental::Sharp || nn!=NoteName::G || aa!=Accidental::Flat) &&
                                                       (nn!=NoteName::F || aa!=Accidental::Sharp || n!=NoteName::G || a!=Accidental::Flat) &&
                                                       (nn!=NoteName::G || aa!=Accidental::Sharp || n!=NoteName::A || a!=Accidental::Flat) &&
                                                       (n!=NoteName::G || a!=Accidental::Sharp || nn!=NoteName::A || aa!=Accidental::Flat) &&
                                                       (nn!=NoteName::A || aa!=Accidental::Sharp || n!=NoteName::B || a!=Accidental::Flat) &&
                                                       (n!=NoteName::A || a!=Accidental::Sharp || nn!=NoteName::B || aa!=Accidental::Flat) &&
                                                       (nn!=NoteName::B || aa!=Accidental::None || n!=NoteName::C || a!=Accidental::Flat) &&
                                                       (n!=NoteName::B || a!=Accidental::None || nn!=NoteName::C || aa!=Accidental::Flat) &&
                                                       (nn!=NoteName::E || aa!=Accidental::None || n!=NoteName::F || a!=Accidental::Flat) &&
                                                       (n!=NoteName::E || a!=Accidental::None || nn!=NoteName::F || aa!=Accidental::Flat) &&
                                                       (nn!=NoteName::B || aa!=Accidental::Sharp || n!=NoteName::C || a!=Accidental::None) &&
                                                       (n!=NoteName::B || a!=Accidental::Sharp || nn!=NoteName::C || aa!=Accidental::None) ){
                                                        //cout<<(nn!=n)<<(a!=aa)<<(i!=j)<<(Fraction((int)j,(int)k)!=Fraction((int)jj,(int)kk))<<" "<<Fraction((int)j,(int)k)<<" "<<Fraction((int)jj,(int)kk)<<" "<<(Fraction((int)j,(int)k)<=0)<<" "<<ref.m_duration<<endl;
                                                        REQUIRE(ref!=ref2);
                                                        REQUIRE(!(ref==ref2));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator <<"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    Pitch P(n,a,Octave(i));
                    for(unsigned j = 1;j<6;j++){
                        for(unsigned k = 1;k<6;k++){
                            Note N(P,Fraction((int)j,(int)k));
                            stringstream ss,ss2;
                            stringstream frac;
                            if(Fraction((int)j,(int)k)!=1){
                                if(Fraction((int)j,(int)k).numerator() != 1){
                                    frac<<Fraction((int)j,(int)k).numerator();
                                }
                                if(Fraction((int)j,(int)k).denominator() != 1){
                                    frac<<"/";
                                    frac<<Fraction((int)j,(int)k).denominator();
                                }
                            }
               
                            Octave o(i>4 ? i-1 : i);
                            ss<<a<<n<<strOfOctave(Octave(o))<<frac.str();
                            ss2<<N;
                            string target = ss.str();
                            if(i>4){
                                std::transform(target.begin(),target.end(),target.begin(),::tolower);
                            }
                            REQUIRE(target==ss2.str());
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator >>"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    Pitch P(n,a,Octave(i));
                    for(unsigned j = 1;j<6;j++){
                        for(unsigned k = 1;k<6;k++){
                            Note N(P,Fraction((int)j,(int)k));
                            stringstream ss;
                            ss<<a<<n<<strOfOctave(Octave(i))<<Fraction((int)j,(int)k).numerator()<<"/"<<Fraction((int)j,(int)k).denominator();
                            REQUIRE_NOTHROW( ss>>N);
                            REQUIRE(!ss.fail());
                            REQUIRE(N==Note(n,a,Octave(i),Fraction((int)j,(int)k)));
                            string str;
                            ss>>str;
                            REQUIRE(str=="");
                        }
                    }
                }
            }
        }
    }
    SECTION("Identity >> <<"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(unsigned i=0;i<11;i++){
                    Pitch P(n,a,Octave(i));
                    for(unsigned j = 1;j<6;j++){
                        for(unsigned k = 1;k<6;k++){
                            Note N(P,Fraction((int)j,(int)k)),N2;
                            stringstream ss;
                            ss<<N;
                            REQUIRE_NOTHROW(ss>>N2);
                            REQUIRE(!ss.fail());
                            REQUIRE(N==N2);
                            string str;
                            ss>>str;
                            REQUIRE(str=="");
                        }
                    }
                }
            }
        }
    }
    stringstream ss;
    SECTION("Error handling"){
        SECTION("Wrong note name"){
            ss.clear();
            ss.str(string());
            string str="K~,,1/1",str2;
            ss<<str;
            Note cn;
            REQUIRE_NOTHROW(ss>>cn);
            REQUIRE(ss.fail());
            ss.clear();
            ss>>str2;
            REQUIRE(str==str2);
            ss.clear();
            ss.str(string());
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            ss<<str;
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
            ss.clear();
            ss>>str2;
            REQUIRE(str==str2);
        }
        SECTION("Wrong accidental"){
            ss.clear();
            ss.str(string());
            string str="#E'''1/1",str2;
            ss<<str;
            Pitch cn;
            REQUIRE_NOTHROW(ss>>cn);
            REQUIRE(ss.fail());
            ss.clear();
            ss>>str2;
            REQUIRE(str==str2);
            ss.clear();
            ss.str(string());
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            ss<<str;
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
            ss.clear();
            ss>>str2;
            REQUIRE(str==str2);
        }
        SECTION("Underflow"){
            ss.clear();
            ss.str(string());
            for(auto n : availableNotes){
                for(auto a : availableAccidentals){
                    string s=",,,,",under="";
                    for(unsigned i=5;i<11;i++){
                        under+=",";
                        ss.clear();
                        ss.str(string());
                        ss<<a<<n<<s<<under<<"1/1";
                        string str=ss.str();
                        Note cn;
                        REQUIRE_NOTHROW(ss>>cn);
                        REQUIRE(!ss.fail());
                        REQUIRE(cn==Note(n,a,Octave(0),Fraction(1)));
                        std::string str2;
                        ss>>str2;
                        REQUIRE(string(under+"1/1")==str2);
                    }
                }
            }
        }
        SECTION("Empty string"){
            ss.clear();
            ss.str(string()); //empty string case
            Note cn;
            ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
            REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
        }
        SECTION("Impossible durations"){
            SECTION("Nul denominator"){
                ss.clear();
                ss.str(string());
                string str="C,,1/0";
                ss<<str;
                Note cn;
                REQUIRE_THROWS(ss>>cn);
                string str2;
                /*ss>>str2;
                REQUIRE(str==str2);*/
            }
            SECTION("Nul numerator"){
                ss.clear();
                ss.str(string());
                string str="C,,0/1",str2;
                ss<<str;
                Note cn;
                REQUIRE_NOTHROW(ss>>cn);
                /*REQUIRE(ss.fail());
                ss.clear();
                ss>>str2;
                REQUIRE(str==str2);
                REQUIRE(str==ss.str());*/
                ss.clear();
                ss.str(string());
                ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
                ss<<str;
                REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
                /*ss.clear();
                ss>>str2;
                REQUIRE(str==str2);*/
            }
            SECTION("Negative denominator"){
                ss.clear();
                ss.str(string());
                string str="C,,1/-1",str2;
                ss<<str;
                Note cn;
                REQUIRE_NOTHROW(ss>>cn);
                ss.clear();
                ss>>str2;
                //REQUIRE(str==str2);
                //REQUIRE(str==ss.str());
                ss.clear();
                ss.str(string());
                ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
                ss<<str;
                /*REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
                ss.clear();
                ss>>str2;
                REQUIRE(str==str2);*/
            }
            SECTION("Negative numerator"){
                ss.clear();
                ss.str(string());
                string str="C,,-1/1",str2;
                ss<<str;
                Note cn;
                /*REQUIRE_NOTHROW(ss>>cn);
                REQUIRE(ss.fail());
                ss.clear();
                ss>>str2;
                REQUIRE(str==str2);
                ss.clear();
                ss.str(string());
                ss.exceptions(std::stringstream::failbit | std::stringstream::badbit );
                ss<<str;
                REQUIRE_THROWS_AS(ss>>cn,std::stringstream::failure);
                ss.clear();
                ss>>str2;
                REQUIRE(str==str2);*/
            }
        }
    }

}
#endif

TEST_CASE("Test of Measure class","[Measure]")
{
    SECTION("Construction of the object and setters/getters"){
        Measure M(false);
        REQUIRE(!M.isBinary());
        REQUIRE(M.getDuration()==4);
        REQUIRE_THROWS_AS(Measure MM(false,5),std::domain_error);
        SECTION("isBinary"){
            Fraction deb(3,10);
            Note N(NoteName::C,Accidental::Flat,5, Fraction(2));
            Measure M2(true);
            M.addNote(Measure::Position(1),N);
            REQUIRE(M2.isBinary());
            Measure MM(false);
            REQUIRE(!MM.isBinary());
            M.setBinary(true);
            REQUIRE(M2.isBinary());
            M.setBinary(false);
            REQUIRE(!M.isBinary());
        }
    }
    SECTION("Add note"){
        SECTION("Valid case"){


        }
    }
}


vector<tuple<Chord,vector<string>,string>> majorChords  {
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Sharp)) , vector<string> {"C","E","G"},"C"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::None)) , vector<string> {"C","E","G"},"C"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Sharp)) , vector<string> {"^C","F","^G"},"C#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Flat)) , vector<string> {"^C","F","^G"},"C#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::None)) , vector<string> {"D","^F","A"},"D"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Sharp)) , vector<string> {"^D","G","^A"},"D#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Flat)) , vector<string> {"^D","G","^A"},"D#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::None)) , vector<string> {"E","^G","B"},"E"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Flat)) , vector<string> {"E","^G","B"},"E"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::None)) , vector<string> {"F","A","C"},"F"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Sharp)) , vector<string> {"F","A","C"},"F"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Sharp)) , vector<string> {"^F","^A","^C"},"F#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Flat)) , vector<string> {"^F","^A","^C"},"F#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::None)) , vector<string> {"G","B","D"},"G"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Sharp)) , vector<string> {"^G","C","^D"},"G#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Flat)) , vector<string> {"^G","C","^D"},"G#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::None)) , vector<string> {"A","^C","E"},"A"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Sharp)) , vector<string> {"^A","D","F"},"A#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Flat)) , vector<string> {"^A","D","F"},"A#"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::None)) , vector<string> {"B","^D","^F"},"B"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Flat)) ,vector<string> {"B","^D","^F"},"B")
};


vector<tuple<Chord,vector<string>,string>> minorChords {
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"C","_E","G"},"Cm"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"C","_E","G"},"Cm"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^C","E","^G"},"C#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^C","E","^G"},"C#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"D","F","A"},"Dm"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^D","_G","^A"},"D#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^D","_G","^A"},"D#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"E","G","B"},"Em"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"E","G","B"},"Em"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"F","_A","C"},"Fm"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"F","_A","C"},"Fm"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^F","A","^C"},"F#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^F","A","^C"},"F#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"G","_B","D"},"Gm"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^G","B","^D"},"G#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^G","B","^D"},"G#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"A","C","E"},"Am"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^A","_D","F"},"A#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"^A","_D","F"},"A#m"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"B","D","^F"},"Bm"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Minor) ,vector<string> {"B","D","^F"},"Bm")
};


vector<tuple<Chord,vector<string>,string>> sus4Chords  {
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"C","F","G"},"Csus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"C","F","G"},"Csus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^C","^F","^G"},"C#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^C","^F","^G"},"C#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"D","G","A"},"Dsus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^D","^G","^A"},"D#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^D","^G","^A"},"D#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"E","A","B"},"Esus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"E","A","B"},"Esus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"F","^A","C"},"Fsus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"F","^A","C"},"Fsus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^F","B","^C"},"F#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^F","B","^C"},"F#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"G","C","D"},"Gsus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^G","^C","^D"},"G#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^G","^C","^D"},"G#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"A","D","E"},"Asus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^A","^D","F"},"A#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"^A","^D","F"},"A#sus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus4) , vector<string> {"B","E","^F"},"Bsus4"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus4) ,vector<string> {"B","E","^F"},"Bsus4")
};


vector<tuple<Chord,vector<string>,string>> sus2Chords {
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"C","D","G"},"Csus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"C","D","G"},"Csus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^C","_E","^G"},"C#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^C","_E","^G"},"C#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"D","E","A"},"Dsus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^D","F","^A"},"D#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^D","F","^A"},"D#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"E","_G","B"},"Esus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"E","_G","B"},"Esus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"F","G","C"},"Fsus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"F","G","C"},"Fsus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^F","_A","^C"},"F#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^F","_A","^C"},"F#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"G","A","D"},"Gsus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^G","_B","^D"},"G#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^G","_B","^D"},"G#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"A","B","E"},"Asus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^A","C","F"},"A#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"^A","C","F"},"A#sus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"B","_D","^F"},"Bsus2"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Sus2) ,vector<string> {"B","_D","^F"},"Bsus2")
};



vector<tuple<Chord,vector<string>,string>> dimChords {
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"C","_E","_G"},"Cdim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"C","_E","_G"},"Cdim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^C","E","G"},"C#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^C","E","G"},"C#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"D","F","_A"},"Ddim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^D","_G","A"},"D#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^D","_G","A"},"D#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"E","G","_B"},"Edim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"E","G","_B"},"Edim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"F","_A","B"},"Fdim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"F","_A","B"},"Fdim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^F","A","C"},"F#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^F","A","C"},"F#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"G","_B","_D"},"Gdim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^G","B","D"},"G#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^G","B","D"},"G#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"A","C","_E"},"Adim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^A","_D","E"},"A#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"^A","_D","E"},"A#dim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"B","D","F"},"Bdim"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Dim) ,vector<string> {"B","D","F"},"Bdim")
};



vector<tuple<Chord,vector<string>,string>> augChords  {
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"C","E","^G"},"Caug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"C","E","^G"},"Caug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^C","F","A"},"C#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^C","F","A"},"C#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"D","^F","^A"},"Daug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::D,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^D","G","B"},"D#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^D","G","B"},"D#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"E","^G","C"},"Eaug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"E","^G","C"},"Eaug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"F","A","^C"},"Faug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::E,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"F","A","^C"},"Faug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::F,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^F","^A","D"},"F#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^F","^A","D"},"F#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"G","B","^D"},"Gaug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::G,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^G","C","E"},"G#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^G","C","E"},"G#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"A","^C","F"},"Aaug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::A,Accidental::Sharp),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^A","D","^F"},"A#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"^A","D","^F"},"A#aug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::B,Accidental::None),Chord::SeventhType::None,Chord::ChordType::Aug) , vector<string> {"B","^D","G"},"Baug"),
    make_tuple(Chord(Fraction(4),CompleteNoteName(NoteName::C,Accidental::Flat),Chord::SeventhType::None,Chord::ChordType::Aug) ,vector<string> {"B","^D","G"},"Baug")
};



static const vector<string> minorSeventh {"^A","^A","B","B","C","^C","^C","D","D","^D","^D","E","E","F","^F","^F","G","^G","^G","A","A"};

vector<string> majorSeventh(minorSeventh.size());
vector<string> dimSeventh(minorSeventh.size());

static const vector<vector<tuple<Chord,vector<string>,string>>> chords {minorChords,majorChords,sus4Chords,sus2Chords,dimChords,augChords};

static const vector<Chord::ChordType> availableChords {Chord::ChordType::Major,Chord::ChordType::Minor,Chord::ChordType::Dim,Chord::ChordType::Aug,Chord::ChordType::Sus4,Chord::ChordType::Sus2};

string seventhName(Chord::SeventhType s) __attribute__((const));

string seventhName(Chord::SeventhType s){
    switch(s){
    case Chord::SeventhType::None:
        return "";
    case Chord::SeventhType::Minor:
        return "7m";
    case Chord::SeventhType::Major:
        return "7";
    case Chord::SeventhType::Dim:
        return "7dim";
    }
    assert(false);
    return "";
}

std::ostream& operator <<(std::ostream& o, const Chord::ChordType & c);
std::ostream& operator <<(std::ostream& o, const Chord::ChordType & c){
    switch(c){
    case Chord::ChordType::Major:
        o<<"Major";
        break;
    case Chord::ChordType::Minor:
        o<<"Minor";
        break;
    case Chord::ChordType::Dim:
        o<<"Dim";
        break;
    case Chord::ChordType::Aug:
        o<<"Aug";
        break;
    case Chord::ChordType::Sus4:
        o<<"Sus4";
        break;
    case Chord::ChordType::Sus2:
        o<<"Sus2";
        break;
    }
    return o;
}
std::ostream& operator <<(std::ostream& o, const Chord::SeventhType & c);
std::ostream& operator <<(std::ostream& o, const Chord::SeventhType & c){
    o<<seventhName(c);
    return o;
}
TEST_CASE("Test of Chord","[Chord]")
{
    std::transform(minorSeventh.begin(),minorSeventh.end(),majorSeventh.begin(),[](string a){
        Pitch P;
        stringstream ss;
        ss<<a;
        ss>>P;
        P+=1;
        ss<<P.getNoteName();
        string s;
        ss>>s;
        return s;
    });
    std::transform(minorSeventh.begin(),minorSeventh.end(),dimSeventh.begin(),[](string a){
        Pitch P;
        stringstream ss;
        ss<<a;
        ss>>P;
        P-=1;
        ss<<P.getNoteName();
        string s;
        ss>>s;
        return s;
    });
    static const vector<pair<Chord::SeventhType,vector<string>>> availableSeventh  {
    {Chord::SeventhType::None,vector<string>()},
        {Chord::SeventhType::Minor,minorSeventh},
            {Chord::SeventhType::Major,majorSeventh},
    {Chord::SeventhType::Dim,dimSeventh}};


    SECTION("Operator == and !="){
        for(auto clist : chords){
            for(auto c : clist){
                for(auto s : availableSeventh){
                    for(auto cc : clist){
                        for(auto ss : availableSeventh){
                            get<0>(c).m_seventh=s.first;
                            get<0>(cc).m_seventh=ss.first;
                            if(ss.first==s.first&&get<2>(c)==get<2>(cc)){
                                REQUIRE(get<0>(c)==get<0>(cc));
                                REQUIRE(!(get<0>(c)!=get<0>(cc)));
                            }else{
                                REQUIRE(get<0>(c)!=get<0>(cc));
                                REQUIRE(!(get<0>(c)==get<0>(cc)));
                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("Operator ==, special cases"){
        for(auto n : availableNotes){
            for(auto a : availableAccidentals){
                for(auto s : availableSeventh){
                    for(auto c : availableChords){
                        for(auto nn : availableNotes){
                            for(auto aa : availableAccidentals){
                                CompleteNoteName cn1(n,a),cn2(nn,aa);
                                if(cn1==cn2){
                                    REQUIRE(Chord(Fraction(4),cn1,s.first,c)==Chord(Fraction(4),cn2,s.first,c));
                                }else{
                                    REQUIRE(Chord(Fraction(4),cn1,s.first,c)!=Chord(Fraction(4),cn2,s.first,c));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    SECTION("ReadChord"){
        for(auto clist : chords){
            for(auto seventh : availableSeventh){
                for(auto c : clist){
                    Chord C;
                    get<0>(c).m_seventh=seventh.first;
                    string name = get<2>(c) + seventhName(seventh.first);
                    REQUIRE_NOTHROW(C=readChord(name));
                    CAPTURE(name);
                    CAPTURE(get<0>(c).m_seventh);
                    CAPTURE(get<0>(c).m_base);
                    CAPTURE(get<0>(c).m_duration);
                    CAPTURE(get<0>(c).m_type);
                    CAPTURE(C.m_seventh);
                    CAPTURE(C.m_base);
                    CAPTURE(C.m_duration);
                    CAPTURE(C.m_type);
                    CAPTURE(seventh.first);
                    INFO(seventhName(seventh.first));
                    REQUIRE(C==get<0>(c));;
                }
            }
        }

    }
    SECTION("Operator <<"){
        for(auto clist : chords){
            for(auto seventh : availableSeventh){
                for(auto c : clist){
                    get<0>(c).m_seventh=seventh.first;
                    stringstream ss;
                    REQUIRE_NOTHROW(ss<<get<0>(c));
                    REQUIRE(ss.str()==get<2>(c) + seventhName(seventh.first));
                }
            }
        }
    }
    SECTION("Fixed point << readChord"){
        for(auto clist : chords){
            for(auto seventh : availableSeventh){
                for(auto c : clist){
                    get<0>(c).m_seventh=seventh.first;
                    Chord C;
                    stringstream ss;
                    REQUIRE_NOTHROW(ss<<get<0>(c));
                    CAPTURE(ss.str());
                    REQUIRE_NOTHROW(C=readChord(ss.str()));
                    REQUIRE(C==get<0>(c));;
                }
            }
        }
    }
    SECTION("Bad input"){
        Chord C;
        REQUIRE_THROWS(C=readChord("K#m"));
        REQUIRE_THROWS(C=readChord("C^m"));
        REQUIRE_THROWS(C=readChord("Csus2sus4"));
    }
    SECTION("getNotes"){
        for(auto clist : chords){
            for(auto seventh : availableSeventh){
                unsigned compt=0;
                for(auto c : clist){
                    get<0>(c).m_seventh=seventh.first;
                    CAPTURE(get<0>(c));
                    INFO((seventh.first==Chord::SeventhType::Minor ? "minor seventh" : "other"));
                    INFO((seventh.first==Chord::SeventhType::None ? "None seventh" : "other"));
                    auto expected = get<1>(c);
                    if(seventh.first!=Chord::SeventhType::None){
                        expected.push_back(seventh.second[compt]);
                    }
                    auto res = get<0>(c).getNotes();
                    REQUIRE(expected.size()==res.size());
                    for(unsigned i = 0;i<res.size();i++){
                        stringstream debug;
                        INFO(debug.str());
                        CompleteNoteName cn;
                        stringstream ss;
                        ss<<expected[i];
                        ss>>cn;
                        REQUIRE(cn==res[i]);
                    }
                    compt++;
                }
            }
        }

    }
}
/*
  TEST_CASE("Test of Melody class","[Melody]")
  {
  SECTION("Construction of the object and setters/getters"){

  SECTION("getKey"){
  Key k(NoteName::D, NoteName::F);
  Melody M(vector<Measure> (), k);
  Key kk = M.getKey();
  REQUIRE(kk.m_base==k.m_base);
  REQUIRE(kk.m_mode==k.m_mode);
  k.m_base = NoteName::E;
  k.m_mode = NoteName::G;
  kk = M.getKey();
  REQUIRE(kk.m_base!=k.m_base);
  REQUIRE(kk.m_mode!=k.m_mode);

  }
  SECTION("getMeasures"){
  FAIL("Requires a valid and tested == operator for measures.");
  REQUIRE(false);
  Key k( NoteName::D, NoteName::F);
  Fraction deb(0.3);
  Note N(NoteName::C,Accidental::Flat,5, Fraction(2));
  Measure M(1,true);
  M.addNote(Measure::Position(1),N);
  vector<Measure> vm,vmb;
  vm.push_back(M);
  Melody Mel(vm,k);
  vmb = Mel.getMeasures();
  REQUIRE(vm.size()==vmb.size());
  for(unsigned i = 0;i<vm.size();i++){
  //REQUIRE(vm[i]==vmb[i]);
  WARN("Decomment line when operator is defined");
  }

  }

  }
  }

*/
