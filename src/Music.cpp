#include "Music.hpp"
#include <ostream>
#include <istream>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include "Utilities.hpp"

using namespace std;

//Applies modification to length and Accidentals
static std::function<void(Note&)> modifier = std::function<void(Note&)>([](Note&){});
static std::function<void(Note&)> inverseModifier = std::function<void(Note&)>([](Note&){});

static bool isEqual = false;
static bool maj = false;
static bool writeMaj = true;

static unsigned noteToPlace(const NoteName& n) __attribute__((const));
static inline unsigned noteToPlace(const NoteName& n)
{
    if(n==NoteName::C)
        return 0;
    else if(n==NoteName::D)
        return 1;
    else if(n==NoteName::E)
        return 2;
    else if(n==NoteName::F)
        return 3;
    else if(n==NoteName::G)
        return 4;
    else if(n==NoteName::A)
        return 5;
    else if(n==NoteName::B)
        return 6;
    return INT_MAX;
}
static const std::vector<NoteName> placeToNote =
    {NoteName::C, NoteName::D, NoteName::E, NoteName::F, NoteName::G, NoteName::A, NoteName::B};

static const std::vector<unsigned> distanceBetweenNotesInC =
    {2, 2, 1, 2, 2, 2, 1};

static std::function<void(CompleteNoteName&)> getAccidentalsModifier(const Key& k);
static std::function<void(CompleteNoteName&)> getAccidentalsModifier(const Key& k)
{
    return std::function<void(CompleteNoteName&)>([k](CompleteNoteName& c){
            if (c.m_modifier != Accidental::None)
                return ;
            if (isEqual)
                return ;

            unsigned pureNote = noteToPlace(c.m_name);
            unsigned pureMode = noteToPlace(k.m_mode);
            unsigned pureBase = noteToPlace(k.m_base.m_name);

            Pitch pref(k.m_base);

            for (unsigned i = 0; i < (pureNote - pureBase + 7) % 7; i++){
                pref += (int)distanceBetweenNotesInC[((pureMode + i) % 7)];
            }

            if (!(pref.m_name == c.m_name && pref.m_modifier == c.m_modifier)){
                if (pref.m_name == c.m_name){
                    if (pref.m_modifier != Accidental::None)
                        c.m_modifier = pref.m_modifier;
                    else
                        // we shouldn't arrive here
                        throw runtime_error("Shouldn't happen");
                }else{
                    pref += 1;
                    if (pref.m_name == c.m_name && pref.m_modifier == c.m_modifier)
                        c.m_modifier = Accidental::Flat;
                    else{
                        pref -= 2;
                        if (pref.m_name == c.m_name && pref.m_modifier == c.m_modifier)
                            c.m_modifier = Accidental::Sharp;
                        else
                            throw runtime_error("Key not supported (double sharps or double flats)");
                    }
                }
            }

        });
}

std::function<void(CompleteNoteName&)> getInverseAccidentalsModifier(const Key& k);
std::function<void(CompleteNoteName&)> getInverseAccidentalsModifier(const Key& k)
{
    return std::function<void(CompleteNoteName&)>([k](CompleteNoteName& c){
            CompleteNoteName cPure(c.m_name,Accidental::None);
            getAccidentalsModifier(k)(cPure);
            if (cPure == c){
                c.m_modifier = Accidental::None;
                isEqual = false;
            }else if (c.m_modifier == Accidental::None){
                isEqual = true;
            }else
                isEqual = false;

        });
}

std::ostream& operator <<(std::ostream& o, NoteName n)
{
    if(writeMaj)
    {
        switch(n){
        case NoteName::A :
            return o<<'A';
        case NoteName::B :
            return o<<'B';
        case NoteName::C :
            return o<<'C';
        case NoteName::D :
            return o<<'D';
        case NoteName::E :
            return o<<'E';
        case NoteName::F :
            return o<<'F';
        case NoteName::G :
            return o<<'G';
        }
    }
    else
    {
        switch(n){
        case NoteName::A :
            return o<<'a';
        case NoteName::B :
            return o<<'b';
        case NoteName::C :
            return o<<'c';
        case NoteName::D :
            return o<<'d';
        case NoteName::E :
            return o<<'e';
        case NoteName::F :
            return o<<'f';
        case NoteName::G :
            return o<<'g';
        }
    }

    return o;
}

std::istream& operator >>(std::istream& i, NoteName& n)
{
    char c;
    if(i>>c){
        switch(c){
        case 'A' :
        case 'a' :
            n = NoteName::A;
            break;
        case 'B' :
        case 'b' :
            n = NoteName::B;
            break;
        case 'C' :
        case 'c' :
            n = NoteName::C;
            break;
        case 'D' :
        case 'd' :
            n = NoteName::D;
            break;
        case 'E' :
        case 'e' :
            n = NoteName::E;
            break;
        case 'F' :
        case 'f' :
            n = NoteName::F;
            break;
        case 'G' :
        case 'g' :
            n = NoteName::G;
            break;
        default :
            i.putback(c);
            i.setstate(ios_base::failbit);
        }
    }
    maj = c>='A' && c<='G';
    return i;
}

std::ostream& operator <<(std::ostream& o, Accidental a)
{
    switch(a){
    case Accidental::Sharp :
        return o<< '^';
    case Accidental::Flat :
        return o<< '_';
    case Accidental::None :
        if(isEqual)
            o<<'=';
        return o;
    }
    return o;
}

std::istream& operator >>(std::istream& i, Accidental& a)
{
    char c;
    if(i>>c){
        switch(c){
        case '^' :
            a = Accidental::Sharp;
            break;
        case '_' :
            a = Accidental::Flat;
            break;
        case '=' :
            isEqual = true;
            a = Accidental::None;
            break;
        default :
            isEqual = false;
            a =Accidental::None;
            i.putback(c);
        }
    }else{
        isEqual = false;
        a = Accidental::None;
        i.clear(i.failbit);
        i.clear(i.badbit);
        i.clear(i.goodbit);
    }
    return i;
}

std::ostream& operator <<(std::ostream& o, const CompleteNoteName& n)
{
    return o<<n.m_modifier<<n.m_name;
}

std::istream& operator >>(std::istream& i, CompleteNoteName& n)
{
    return i>>n.m_modifier>>n.m_name;
}

std::ostream& operator <<(std::ostream& o, const Pitch& n)
{
    if(n.m_octave > 4)
        writeMaj=false;
    else
        writeMaj=true;
    o<<static_cast<const CompleteNoteName&>(n);
    writeMaj=true;
    if(n.m_octave >4){
        for(int i=0;i<static_cast<int>(n.m_octave)-5;i++){
            o<<'\'';
        }
    }else{
        for(int i=0;i<4-static_cast<int>(n.m_octave);i++){
            o<<',';
        }
    }
    return o;
}

std::istream& operator >>(std::istream& i, Pitch& n)
{
    if(!(i>>static_cast<CompleteNoteName&>(n)))
        return i;
    Octave octave =4;
    if(!maj)
        octave++;
    char c;
    char r;
    if(i>>c){
        if(c!=',' && c!='\''){
            i.putback(c);
            n.m_octave = octave;
            return i;
        }else{
            while(1){
                if(c==','){
                    octave--;
                    if(octave==0)
                        break;
                }else
                    octave++;
                if(i>>r){
                    if(r!=c){
                        i.putback(r);
                        break;
                    }
                }else{
                    i.clear(i.failbit);
                    i.clear(i.badbit);
                    i.clear(i.goodbit);
                    break;
                }
            }

        }
    }else{
        i.clear(i.failbit);
        i.clear(i.badbit);
        i.clear(i.goodbit);
    }
    n.m_octave=octave;
    return i;
}

void writeDuration(std::ostream& o, const Fraction& f);
void writeDuration(std::ostream& o, const Fraction& f)
{
#ifdef BOOST
    if(f.numerator()!=1)
        o<<f.numerator();
    if(f.denominator()!=1)
        o<<"/"<<f.denominator();
#else
    throw std::runtime_error("You need boost to call this function (write Duration)");
#endif
}

std::ostream& operator <<(std::ostream& o, const Note& n)
{
    Note copy(n);
    inverseModifier(copy);
    o<<static_cast<const Pitch&>(copy);
    writeDuration(o, copy.m_duration);
    return o;
}
Fraction readDuration(std::istream& i);

unsigned readUnsigned(std::istream& i);
unsigned readUnsigned(std::istream& i)
{
    unsigned num = std::numeric_limits<unsigned>::max();
    char c;
    while(i>>c){
        if(c >='0' && c<='9'){
            if(num==std::numeric_limits<unsigned>::max())
                num=0;
            num*=10;
            num+=static_cast<unsigned>(c-'0');
        }else{
            i.putback(c);
            return num;
        }
    }
    i.clear(i.failbit);
    i.clear(i.badbit);
    i.clear(i.goodbit);
    return num;
}

Fraction readDuration(std::istream& i)
{
    unsigned num;
    unsigned denom;
    num = readUnsigned(i);
    if(num==std::numeric_limits<unsigned>::max())
        num=1;
    char c='.';

    if(i>>c){
        if(c!='/'){
            i.putback(c);
            denom =1;
        }else{
            denom = readUnsigned(i);
            if(denom==std::numeric_limits<unsigned>::max())
                denom = 2;
        }
    }else{
        i.clear(i.failbit);
        i.clear(i.badbit);
        i.clear(i.goodbit);
        denom = 1;
    }
    return Fraction(static_cast<int>(num), static_cast<int>(denom));
}

std::istream& operator >>(std::istream& i, Note& n)
{
    if(!(i>>static_cast<Pitch&>(n)))
        return i;
    n.m_duration = readDuration(i);
    modifier(n);
    return i;
}

std::ostream& operator <<(std::ostream& o, const Measure& m)
{
    Fraction pos = 0;
    for(auto it : m){
        if(it.first!=pos){
            o<<"z";
            writeDuration(o,it.first-pos);
            pos=it.first;
            o<<" ";
        }
        o<<it.second;
        pos+=it.second.m_duration;
        if(pos!=static_cast<int>(m.getDuration()))
            o<<" ";
    }
    if(pos!=Fraction(static_cast<int>(m.getDuration()))){
        o<<"z";
        writeDuration(o,static_cast<int>(m.getDuration())-pos);
    }
    return o;
}

std::istream& operator >>(std::istream& i, Measure& m)
{
    m = Measure(m.isBinary(), m.getDuration());
    Fraction pos = 0;
    while(pos<static_cast<int>(m.getDuration())){
        Note n;
        //i.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
        if(!(i>>n)){
            i.clear(i.failbit);
            i.clear(i.badbit);
            i.clear(i.goodbit);
            char c;
            if(i>>c){
                if(c=='z'){
                    Fraction time = readDuration(i);
                    Note tmp;
                    tmp.m_duration=time;
                    modifier(tmp);
                    pos+=tmp.m_duration;
                }else{
                    i.setstate(ios_base::failbit);
                    break;
                }
            }else{
                i.setstate(ios_base::failbit);
                break;
            }
        }else{
            m.addNote(pos, n);
            pos+=n.m_duration;
        }
    }
    return i;
}

std::ostream& operator <<(std::ostream& o, const Key& k)
{
    o<<k.m_base.m_name;
    switch(k.m_base.m_modifier){
    case Accidental::Flat :
        o<<'b';
        break;
    case Accidental::Sharp :
        o<<'#';
        break;
    case Accidental::None :
        break;
    }
    switch(k.m_mode){
    case NoteName::A :
        o<<"m";
        break;
    case NoteName::B :
        o<<"Loc";
        break;
    case NoteName::C :
        break;
    case NoteName::D :
        o<<"Dor";
        break;
    case NoteName::E :
        o<<"Phr";
        break;
    case NoteName::F :
        o<<"Lyd";
        break;
    case NoteName::G :
        o<<"Mix";
        break;
    }
    return o;
}

Key readKey(const std::string& str)
{
    Key res;
    if(str.size()==0)
        throw std::runtime_error("No characters to read Key from");
    else{
        stringstream s;
        s<<str[0];
        if(!(s>>res.m_base.m_name))
            throw std::runtime_error("Base not incorrect");
        if(str.size()==1){
            res.m_base.m_modifier = Accidental::None;
            res.m_mode = NoteName::C;
            return res;
        }
        std::string mode;
        if(str[1]=='b'){
            res.m_base.m_modifier = Accidental::Flat;
            mode = str.substr(2);
        }else if(str[1]=='#'){
            res.m_base.m_modifier = Accidental::Sharp;
            mode = str.substr(2);
        }else
            mode = str.substr(1);
        std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
        if(mode=="m")
            res.m_mode = NoteName::A;
        else if(mode == "loc")
            res.m_mode = NoteName::B;
        else if(mode == "")
            res.m_mode = NoteName::C;
        else if(mode == "dor")
            res.m_mode = NoteName::D;
        else if(mode == "phr")
            res.m_mode = NoteName::E;
        else if(mode == "lyd")
            res.m_mode = NoteName::F;
        else if(mode == "mix")
            res.m_mode = NoteName::G;
        else
            throw std::runtime_error("Mode not in mode list");
        return res;
    }
}

std::ostream& operator <<(std::ostream& o, const Chord& c)
{
    o<<c.m_base.m_name;
    switch(c.m_base.m_modifier){
    case Accidental::Flat :
        o<<'b';
        break;
    case Accidental::Sharp :
        o<<'#';
        break;
    case Accidental::None :
        break;
    }
    switch(c.m_type){
    case Chord::ChordType::Minor :
        o<<'m';
        break;
    case Chord::ChordType::Major :
        break;
    case Chord::ChordType::Sus2 :
        o<<"sus2";
        break;
    case Chord::ChordType::Sus4 :
        o<<"sus4";
        break;
    case Chord::ChordType::Aug :
        o<<"aug";
        break;
    case Chord::ChordType::Dim :
        o<<"dim";
        break;
    }
    switch(c.m_seventh){
    case Chord::SeventhType::Dim :
        o<<"7dim";
        break;
    case Chord::SeventhType::Major :
        o<<"7M";
        break;
    case Chord::SeventhType::Minor :
        o<<"7";
        break;
    case Chord::SeventhType::None :
        break;
    }
    return o;
}


Chord readChord(const std::string& str)
{
    std::map<std::string, NoteName> notes ={{"a", NoteName::A},{"b", NoteName::B},{"c", NoteName::C},{"d", NoteName::D},{"e", NoteName::E},{"f", NoteName::F},{"g", NoteName::G}
    };
    std::map<std::string, Accidental> acc = {{"b", Accidental::Flat}, {"#", Accidental::Sharp}, {"", Accidental::None}};
    std::map<std::string, Chord::ChordType> ct ={{"", Chord::ChordType::Major}, {"m", Chord::ChordType::Minor},{"dim", Chord::ChordType::Dim}, {"aug", Chord::ChordType::Aug},{"sus2", Chord::ChordType::Sus2}, {"sus4", Chord::ChordType::Sus4}};
    std::map<std::string, Chord::SeventhType> st ={{"7M", Chord::SeventhType::Major}, {"7", Chord::SeventhType::Minor},{"7dim", Chord::SeventhType::Dim}, {"", Chord::SeventhType::None}};
    std::string lower =str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for(auto n : notes)
        for(auto c : acc)
            for(auto t : ct)
                for(auto s : st){
                    if(lower == n.first+c.first+t.first+s.first){
                        return Chord(4, CompleteNoteName(n.second, c.second), s.second, t.second);
                    }
                }
    throw std::runtime_error("The string does not match a chord");
}

std::ostream& operator <<(std::ostream& o, const ChordProgression& c)
{
    for(auto it=c.begin(); it!=c.end();it++){
        o<<*it<<";";
    }
    return o;
}

ChordProgression readChordProgression(const std::string& str)
{
    std::string c;
    ChordProgression res;
    for(unsigned i=0;i<str.length();i++){
        if(str[i]!=';')
            c+=str[i];
        else{
            res.push_back(readChord(c));
            c="";
        }
    }
    return res;
}

std::ostream& operator <<(std::ostream& o, const Melody& m)
{
    o<<"X:1\n";
    if(m.isBinary())
        o<<"M:4/4\n";
    else
        o<<"M:12/8\n";
    o<<"L:1/4\n";
    o<<"K:"<<m.getKey()<<"\n";
    o<<"Q:"<<toString(m.m_BPM)<<"\n";
    auto accidentalModifier = getInverseAccidentalsModifier(m.getKey());
    inverseModifier = std::function<void(Note&)>([&accidentalModifier](Note& c){accidentalModifier(c);});
    try{
        for(auto measure : m){
            o<<measure<<" | ";
        }
        //o<<"]";
        isEqual = false;
        inverseModifier = std::function<void(Note&)>([](Note&){});
        return o;
    }
    catch(...){
        isEqual = false;
        inverseModifier = std::function<void(Note&)>([](Note&){});
        throw;
    }
}

Melody readMelody(const std::string& str)
{
    string s = str;
    s.erase(remove_if(s.begin(), s.end(),[](char c){return (isspace(c) && c != '\n');}), s.end());
    long unsigned pos = 0;
    bool binary = true;
    Key k;
    Fraction unitOfMeasure = Fraction(1,8);
    //Reading Header
    unsigned tempo = 120;
    while(1){
        if(s.size()<pos+2)
            break;
        if(s[pos+1] != ':')
            break;
        string line = s.substr(pos+2, s.find('\n', pos+2) - pos - 2);
        Fraction r;
        istringstream p(line);
        switch(s[pos]){
        case 'M' :
            p >> r;
            if(r==Fraction(12,8)){
                binary = false;
            }

            break;
        case 'K' :
            k = readKey(line);
            break;
        case 'L' :
            p>>unitOfMeasure;
            break;
        case 'Q' :
            p>>tempo;
        default :
            break;
        }
        pos = s.find('\n', pos+2)+1;
    }
    std::string content = s.substr(pos);
    string toIgnore = "()|[]<>:-";
    content.erase(remove_if(content.begin(), content.end(), [toIgnore](char c){return isspace(c) || toIgnore.find(c)!=std::string::npos;}), content.end());
    std::vector<Measure> res;
    istringstream cstream(content);
    std::function<void(CompleteNoteName&)> macc = getAccidentalsModifier(k);
    modifier = std::function<void(Note&)>([unitOfMeasure, &macc](Note& c) {c.m_duration*=4*unitOfMeasure; macc(c);});
    try{
        Measure m(binary);
        while(cstream){
            try{
                cstream>>m;
            }
            catch(const std::runtime_error& e){
                ostringstream c;
                c<<(res.size()+1);
                throw std::runtime_error("When reading melody, on measure number " + c.str() + " : " + e.what());
            }
            catch(...){
                ostringstream c;
                c<<(res.size()+1);
                throw std::runtime_error("When reading melody, on measure number " + c.str() + " : Unknown exception");
            }
            if(cstream)
                res.push_back(m);
        }
        modifier = std::function<void(Note&)>([unitOfMeasure](Note&) {});
        isEqual = false;
        return Melody(res, k, tempo);
    }
    catch(...){
        modifier =std::function<void(Note&)>([unitOfMeasure](Note&) {});
        isEqual = false;
        throw;
    }



}
/*std::istream& operator >>(std::istream& i, ChordProgression& c){
  Chord chord;
  char delim;
  auto state = i.rdstate();
  while(1){
  char c;{
  c.push_back(chord);
  if(i>>delim){
  if(delim!=';'){
  i.putback(delim);
  break;
  }
  }else{
  i.setstate(state);
  break;
  }
  }
  throw std::runtime_error("There was at least one unreadable chord");
  }
  return i;
  }*/

static int noteNameToInt(const NoteName& n) __attribute__((const));
static int noteNameToInt(const NoteName& n)
{
    if (n==NoteName::A)
        return 0;
    else if (n==NoteName::B)
        return 2;
    else if (n==NoteName::C)
        return 3;
    else if (n==NoteName::D)
        return 5;
    else if (n==NoteName::E)
        return 7;
    else if (n==NoteName::F)
        return 8;
    else if (n==NoteName::G)
        return 10;
    return INT_MAX;
}
static int accidentalToInt(const Accidental& a) __attribute__((const));
static int accidentalToInt(const Accidental& a)
{
    if (a==Accidental::Flat)
        return -1;
    else if (a==Accidental::Sharp)
        return 1;
    return 0;
}

unsigned Note::noteToHalfTone(const CompleteNoteName& n)
{
    if(n==CompleteNoteName(NoteName::C))
        return 0;
    else if(n==CompleteNoteName(NoteName::C, Accidental::Sharp))
        return 1;
    else if(n==CompleteNoteName(NoteName::D))
        return 2;
    else if(n==CompleteNoteName(NoteName::D, Accidental::Sharp))
        return 3;
    else if(n==CompleteNoteName(NoteName::E))
        return 4;
    else if(n==CompleteNoteName(NoteName::F))
        return 5;
    else if(n==CompleteNoteName(NoteName::F, Accidental::Sharp))
        return 6;
    else if(n==CompleteNoteName(NoteName::G))
        return 7;
    else if(n==CompleteNoteName(NoteName::G, Accidental::Sharp))
        return 8;
    else if(n==CompleteNoteName(NoteName::A))
        return 9;
    else if(n==CompleteNoteName(NoteName::A, Accidental::Sharp))
        return 10;
    else if(n==CompleteNoteName(NoteName::B))
        return 11;
    return INT_MAX;
}

static const std::vector<CompleteNoteName> halfToneToNote ={CompleteNoteName(NoteName::C), CompleteNoteName(NoteName::C, Accidental::Sharp),
                                                            CompleteNoteName(NoteName::D), CompleteNoteName(NoteName::D, Accidental::Sharp),
                                                            CompleteNoteName(NoteName::E),
                                                            CompleteNoteName(NoteName::F), CompleteNoteName(NoteName::F, Accidental::Sharp),
                                                            CompleteNoteName(NoteName::G), CompleteNoteName(NoteName::G, Accidental::Sharp),
                                                            CompleteNoteName(NoteName::A), CompleteNoteName(NoteName::A, Accidental::Sharp),
                                                            CompleteNoteName(NoteName::B)};

CompleteNoteName::CompleteNoteName(NoteName n,Accidental mod) : m_name(n),m_modifier(mod)
{
}

bool CompleteNoteName::operator==(const CompleteNoteName& o) const
{
    int value1 = accidentalToInt(o.m_modifier)+noteNameToInt(o.m_name);
    /*value1 %= 12;
    if(value1 < 0)
        value1 += 12;*/
    int value2 = accidentalToInt(this->m_modifier)+noteNameToInt(this->m_name);
    /*value2 %= 12;
    if (value2 < 0)
        value2 += 12;*/
    return (value1 - value2)%12==0;
}

bool CompleteNoteName::operator!=(const CompleteNoteName& o) const
{
    return !((*this)==o);
}

Pitch::Pitch(const CompleteNoteName& n, Octave o) : CompleteNoteName(n),m_octave(o)
{
}
Pitch::Pitch(NoteName n, Accidental mod, Octave o) : CompleteNoteName(n,mod),m_octave(o)
{
}
CompleteNoteName Pitch::getNoteName() const
{
    return CompleteNoteName(m_name,m_modifier);
}
Pitch& Pitch::operator+=(int nb)
{
    unsigned level = Note::noteToHalfTone(*this);
    if(nb >= 0){
        m_octave += ((unsigned)nb)/12;
        nb = (nb%12);
    }else{
        unsigned dimin = ((unsigned) (-nb))/12;
        nb = (nb%12);
        if (static_cast<int>(12*m_octave + level) >= 12*static_cast<int>(dimin) - nb){

            if (m_octave >= dimin + 1){
                m_octave -= dimin + 1;
                nb = nb + 12;
            }else{
                m_octave -= dimin;
            }
        }else{
            throw std::domain_error("Note too low, it isn't representable");
        }
    }

    if (nb >= 0){
        if(level+(unsigned)nb > 11){
            m_octave++;
            m_name = halfToneToNote[(unsigned)(level+(unsigned)nb -12)].m_name;
            m_modifier = halfToneToNote[(unsigned)(level+(unsigned)nb -12)].m_modifier;
        }else{
            m_name = halfToneToNote[(unsigned)(level+(unsigned)nb)].m_name;
            m_modifier = halfToneToNote[(unsigned)(level+(unsigned)nb)].m_modifier;
        }
    }else{
        assert (static_cast<int>(level) >= -nb);
        if(nb >= 0)
            level += static_cast<unsigned>(nb);
        else
            level -= static_cast<unsigned>(-nb);
        m_name = halfToneToNote[(unsigned)(level)].m_name;
        m_modifier = halfToneToNote[(unsigned)(level)].m_modifier;
    }
    return *this;
}

Pitch Pitch::operator+(int nb) const
{
    CompleteNoteName note(m_name, m_modifier);
    Pitch p(note, m_octave);
    p+=nb;
    return p;
}

Pitch& Pitch::operator-=(int nb)
{
    *this+=-nb;
    return *this;
}

Pitch Pitch::operator-(int nb) const
{
    CompleteNoteName note(m_name, m_modifier);
    Pitch p(note, m_octave);
    p-=nb;
    return p;
}

bool Pitch::operator==(const Pitch& o) const
{
    return (m_octave == o.m_octave && CompleteNoteName::operator==(o));
}

bool Pitch::operator!=(const Pitch& o) const
{
    return !(*this == o);
}

Note::Note(const Pitch& n, const Fraction& duration) : Pitch(n), m_duration(duration)
{
    if (duration <= 0)
        throw std::domain_error("The duration of the note is non-positive");
}

Note::Note(NoteName n, Accidental mod, Octave o, const Fraction& duration) : Pitch(n,mod,o), m_duration(duration)
{
    if (duration <= 0)
        throw std::domain_error("The duration of the note is non-positive");
}

bool Note::operator==(const Note& o) const
{
    return (m_duration == o.m_duration && Pitch::operator==(o));
}

bool Note::operator!=(const Note& o) const
{
    return !(*this == o);
}

bool Key::operator==(const Key& k) const
{
    return m_base==k.m_base && m_mode==k.m_mode;
}

bool Key::operator!=(const Key& k) const
{
    return !(*this == k);
}

/*

//Implementing the functions of the class Measure
//Constructor

Measure::Measure(std::map<Position, Note> m,unsigned d,bool b) : m_starts(m),m_duration(d),m_binary(b)
{
}
*/
Measure::Measure(bool binary, unsigned duration) : m_starts(), m_duration(duration), m_binary(binary)
{
    if(m_duration!=4)
        throw std::domain_error("We only handle 4 beat measure atm");
}

static bool nonEmptyIntersection(const Fraction& e1, const Fraction& e2, const Fraction& f1, const Fraction& f2);
static inline bool nonEmptyIntersection(const Fraction& e1, const Fraction& e2, const Fraction& f1, const Fraction& f2)
{
    if (e1 > e2 || f1 > f2)
        throw domain_error("Not intervals in input");
    return ((f1 < e1 && f2 > e1) ||
            (f1 < e2 && f2 > e2) ||
            (f1 >= e1 && f1 < e2) ||
            (f2 > e1 && f2 <= e2));
}

void Measure::addNote(const Position& p, const Note& n)
{
    // First we test that the position is possible
    if (p + n.m_duration >  (int)m_duration || p < 0)
        throw domain_error("Position invalid");

    // Then we check that there isn't a note in our interval
    for (ConstIterator iterator = begin(); iterator != end(); iterator++){

        if (nonEmptyIntersection(p, p + n.m_duration, iterator->first, iterator->first + iterator->second.m_duration))
            throw runtime_error("Already a note there");
    }

    (void)m_starts.insert(pair<Position,Note>(p,n));
}

// Get_function

Measure::ConstIterator Measure::begin() const
{
    return m_starts.cbegin();
}

Measure::ConstIterator Measure::end() const
{
    return m_starts.cend();
}

Measure::ConstIterator Measure::getNoteBeingPlayedAt(const Position& p) const
{
    for (Measure::ConstIterator note = Measure::begin(); note != Measure::end(); note++) {
        if (note->first <= p && note->first + note->second.m_duration > p)
            return note;
    }
    return Measure::end();
}

unsigned Measure::getDuration() const
{return m_duration;}

bool Measure::isBinary() const
{return m_binary;}

void Measure::setBinary(bool b)
{m_binary=b;}

bool Measure::operator==(const Measure& o) const
{
    ConstIterator iterator_o = o.begin();

    for (ConstIterator iterator = begin(); iterator != end(); iterator++){
        if (iterator_o == o.end())
            return 0;
        if (iterator->first != iterator_o->first ||
            iterator->second != iterator_o->second)
            return 0;
        iterator_o++;
    }
    return (m_duration == o.m_duration &&
            m_binary == o.m_binary);
}

bool Measure::operator!=(const Measure& o) const
{
    return !(*this == o);
}
/*
// Set_functions

void Measure::setStartingPos(std::map<Fraction, Note> m)
{m_starts=m;}

void Measure::changeDuration(unsigned d)
{m_duration=d;}
*/

Key::Key(const CompleteNoteName& base, const NoteName& mode) : m_base(base), m_mode(mode)
{
}

vector<CompleteNoteName> Key::getNotes() const {
    vector<CompleteNoteName> listOfNotes;
    listOfNotes.push_back(m_base);
    unsigned currentNote = Note::noteToHalfTone(m_base);
    for (unsigned i=0; i<6; i++) {
        currentNote += distanceBetweenNotesInC[(noteToPlace(m_mode)+i)%7];
        currentNote %= 12;
        listOfNotes.push_back(halfToneToNote[currentNote]);
    }
    return listOfNotes;
}

void Key::normalize(CompleteNoteName& n) const
{
    // We assume we use this function only when we transpose from a key without accidentals to this one
    if (n.m_modifier != Accidental::None)
    {
	unsigned pureNote = noteToPlace(n.m_name);
	unsigned pureMode = noteToPlace(m_mode);
	unsigned pureBase = noteToPlace(m_base.m_name);

	Pitch pref(m_base);
	int distBefore;
	int distAfter;

	for (unsigned i = 0; i < (pureNote - pureBase + 7) % 7; i++)
	    pref += (int)distanceBetweenNotesInC[(pureMode + i) % 7];

	unsigned indice = (pureNote - pureBase + 7) % 7;
	distAfter = (int)distanceBetweenNotesInC[(pureMode + indice) % 7];
	distBefore = (int)distanceBetweenNotesInC[(pureMode + indice - 1 + 7) % 7];

	Pitch pNote(n);
	pref.m_octave = pNote.m_octave;

	if (pref != pNote)
	{
	    pref += distAfter;
	    pref.m_octave = pNote.m_octave;
	    if (pref == pNote)
	    {	
		assert(n.m_modifier == Accidental::Sharp);
		n.m_name = placeToNote[(noteToPlace(n.m_name) + 1) % 7];
		n.m_modifier = Accidental::Flat;
	    }
	    else
	    {
		pref -= distAfter + distBefore;
		pref.m_octave = pNote.m_octave;
		if (pref == pNote)
		{
		    assert(n.m_modifier == Accidental::Flat);
		    n.m_name = placeToNote[(noteToPlace(n.m_name) - 1 + 7) % 7];
		    n.m_modifier = Accidental::Sharp;
		}
	    }
	}
    }
}

Chord::Chord(Fraction d, const CompleteNoteName& b, const SeventhType& s, const ChordType& c) : m_duration(d), m_base(b), m_seventh(s), m_type(c)
{
}

bool Chord::operator==(const Chord& o) const
{
    return (m_duration == o.m_duration &&
            m_base == o.m_base &&
            m_seventh == o.m_seventh &&
            m_type == o.m_type);
}

bool Chord::operator!=(const Chord& o) const
{
    return !(*this == o);
}

// Implementing the functions of the class Melody.
//Constructor
Melody::Melody(const vector<Measure>& v,Key k, unsigned tempo) : m_BPM(tempo), m_measures(v),m_k(k)
{
}

Melody::Melody(const Key& k) : m_BPM(120), m_measures(), m_k(k)
{
}
// Get_function

vector<Measure> Melody::convertToMeasures() const
{return vector<Measure>(m_measures);}

Melody::ConstIterator Melody::begin() const{
    return m_measures.begin();
}
Melody::ConstIterator Melody::end() const{
    return m_measures.end();
}


bool Melody::operator==(const Melody& o) const
{
    ConstIterator iterator_o = o.begin();

    for (ConstIterator iterator = begin(); iterator != end(); iterator++){
        if (iterator_o == o.end())
            return 0;
        if (*iterator != *iterator_o)
            return 0;
        iterator_o++;
    }
    return 1;
}

bool Melody::operator!=(const Melody& o) const
{
    return !(*this == o);
}


ChordProgression moveCP(const ChordProgression& cp, int i, const Key& k) {
    unsigned decalage = (unsigned)(i%12+12)%12;
    ChordProgression newcp;
    for (const auto& it : cp) {
	Chord c = Chord(it.m_duration,halfToneToNote[(Note::noteToHalfTone(it.m_base)+decalage)%12],it.m_seventh,it.m_type);
	k.normalize(c.m_base);
        newcp.push_back(c);
    }
    return newcp;
}

vector<CompleteNoteName> Chord::getNotes() const
{
    //TRANSPOSITION
    Chord transposed = Chord(m_duration,CompleteNoteName(NoteName::C,Accidental::None),m_seventh,m_type);

    vector<CompleteNoteName> listOfNotes;
    if (transposed.m_base == CompleteNoteName(NoteName::C, Accidental::None)) { //Just to check it is transposed in C
        CompleteNoteName base;
        base.m_name = NoteName::C;
        base.m_modifier = Accidental::None;
        listOfNotes.push_back(base);
        CompleteNoteName third,fifth;
        //We decide which note is the third of the chord
        if (transposed.m_type == Chord::ChordType::Major || transposed.m_type == Chord::ChordType::Aug){
            third.m_name = NoteName::E;
            third.m_modifier = Accidental::None;
        }else if (transposed.m_type == Chord::ChordType::Minor || transposed.m_type == Chord::ChordType::Dim){
            third.m_name = NoteName::E;
            third.m_modifier = Accidental::Flat;
        }else if (transposed.m_type == Chord::ChordType::Sus2){
            third.m_name = NoteName::D;
            third.m_modifier = Accidental::None;
        }else if (transposed.m_type == Chord::ChordType::Sus4){
            third.m_name = NoteName::F;
            third.m_modifier = Accidental::None;
        }
        listOfNotes.push_back(third);
        fifth.m_name = NoteName::G;
        if (transposed.m_type == Chord::ChordType::Aug) {
            fifth.m_modifier = Accidental::Sharp;
        } else if (transposed.m_type == Chord::ChordType::Dim) {
            fifth.m_modifier = Accidental::Flat;
        } else {
            fifth.m_modifier = Accidental::None;
        }
        listOfNotes.push_back(fifth);
        if (transposed.m_seventh != Chord::SeventhType::None) {
            CompleteNoteName seventh;
            if (transposed.m_seventh == Chord::SeventhType::Major) {
                seventh.m_name = NoteName::B;
                seventh.m_modifier = Accidental::None;
            } else if (transposed.m_seventh == Chord::SeventhType::Minor) {
                seventh.m_name = NoteName::B;
                seventh.m_modifier = Accidental::Flat;
            } else { //Dim
                seventh.m_name = NoteName::A;
                seventh.m_modifier = Accidental::None;
            }
            listOfNotes.push_back(seventh);
        }
    }
    //RETRANSPOSITION
    unsigned decalage = (12-Note::noteToHalfTone(m_base))%12;
    for (unsigned i=0; i<listOfNotes.size(); i++) {
        listOfNotes[i] = halfToneToNote[(Note::noteToHalfTone(listOfNotes[i])-decalage+12)%12];
    }
    return listOfNotes;
}

Melody Melody::transpose(const Key& k) const
{
    std::vector<Measure> v(0);
    int decalage = (int) ((Note::noteToHalfTone(k.m_base)-Note::noteToHalfTone(m_k.m_base)+12)%12);
    for (const auto& m : m_measures){
        Measure t_measure = Measure(m.isBinary(),m.getDuration());
        for (const auto& it2 : m) {
            Note note = Note(it2.second + decalage, it2.second.m_duration);
	    t_measure.addNote(it2.first, note);
        }
        v.push_back(t_measure);
    }
    return Melody(v,k);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=const"
const Key& Melody::getKey() const
{
    return m_k;
}
#pragma GCC diagnostic pop

bool Melody::isBinary() const
{
    if(!m_measures.empty())
        return m_measures[0].isBinary();
    else
        throw std::runtime_error("No measures in Melody. Can not infer whether the melody is binary.");
}

unsigned Melody::size() const
{
    return static_cast<unsigned>(m_measures.size());
}

ostream& Melody::createABCChordFile(ostream& o, const ChordProgression& chords) const
{
    if(chords.size()!=this->size())
        throw std::runtime_error("Melody and Chord Progression do not have same size. Melody size is : " + toString(this->size())+ ".Chord size is : " + toString(chords.size()));
    o<<"X:1\n";
    if(isBinary())
        o<<"M:4/4\n";
    else
        o<<"M:12/8\n";
    o<<"L:1/4\n";
    o<<"K:"<<getKey()<<"\n";
    o<<"T:Chord\n";
    o<<"Q:"+toString(m_BPM)<<"\n";
    auto accidentalModifier = getInverseAccidentalsModifier(getKey());
    inverseModifier = std::function<void(Note&)>([&accidentalModifier](Note& c){accidentalModifier(c);});
    try{
        for(auto it : chords){
            o<<"[";
            for(auto n : it.getNotes()){
                Note l(Pitch(n), 4);
                o<<l;
            }
            o<<"] |";
        }
        isEqual = false;
        inverseModifier = std::function<void(Note&)>([](Note&){});
    }
    catch(...){
        isEqual = false;
        inverseModifier = std::function<void(Note&)>([](Note&){});
        throw;
    }
    return o;
}

Chord::Chord(unsigned id) : Chord()
{
    switch(id%6)
    {
    case 0 :
        m_type = ChordType::Minor;
        break;
    case 1 :
        m_type = ChordType::Major;
        break;
    case 2 :
        m_type = ChordType::Sus4;
        break;
    case 3 :
        m_type = ChordType::Sus2;
        break;
    case 4 :
        m_type = ChordType::Dim;
        break;
    case 5 :
        m_type = ChordType::Aug;
        break;
    }
    id = id/6;
    switch(id%4)
    {
    case 0 :
        m_seventh = SeventhType::Minor;
        break;
    case 1 :
        m_seventh = SeventhType::Major;
        break;
    case 2 :
        m_seventh = SeventhType::Dim;
        break;
    case 3 :
        m_seventh = SeventhType::None;
        break;
    }
    id = id/4;
    m_base = halfToneToNote[id];
}
Chord::operator unsigned() const
{
    unsigned res=Note::noteToHalfTone(m_base);
    res*=4;
    switch(m_seventh)
    {
    case SeventhType::Minor :
        res+=0;
        break;
    case SeventhType::Major :
        res+=1;
        break;
    case SeventhType::Dim :
        res+=2;
        break;
    case SeventhType::None :
        res+=3;
        break;
    }
    res*=6;
    switch(m_type)
    {
    case ChordType::Minor :
        res+=0;
        break;
    case ChordType::Major :
        res+=1;
        break;
    case ChordType::Sus4 :
        res+=2;
        break;
    case ChordType::Sus2 :
        res+=3;
        break;
    case ChordType::Dim :
        res+=4;
        break;
    case ChordType::Aug :
        res+=5;
        break;
    }
    return res;
}
std::vector<std::vector<CompleteNoteName> > chordInit();
std::vector<std::vector<CompleteNoteName> > chordInit()
{
    std::vector<std::vector<CompleteNoteName> > all;
    for(Chord::ChordID i =0;i<4*6*12;i++)
    {
        all.push_back(Chord(i).getNotes());
    }
    return all;
}

const std::vector<CompleteNoteName>& Chord::getNotes(ChordID c)
{
    static std::vector<std::vector<CompleteNoteName> > all = chordInit();
    return all[c];
}
#pragma GCC diagnostic pop
