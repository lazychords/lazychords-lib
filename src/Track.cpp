#include "Track.hpp"
#include <stdexcept>
#include <cstring>

extern "C"
{
#include "fmodex/fmod_errors.h"
#include "ABCToMidi/abc.h"
#include "ABCToMidi/parseabc.h"
#include "fmodex/fmod_memoryinfo.h"
#include "fmodex/fmod_output.h"

}
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "Utilities.hpp"
#include <fstream>


struct FMODSystemWrapper
{
private :
    FMOD_SYSTEM* system;
    static unsigned nb;
public :
    FMODSystemWrapper(const FMODSystemWrapper&) = delete;
    FMODSystemWrapper& operator=(const FMODSystemWrapper&) = delete;
    FMODSystemWrapper() : system(nullptr)
    {
        if(nb>0)
        {
            throw std::runtime_error("Two Instances of FMODSytem are being Created");
        }
        auto result = FMOD_System_Create(&system);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD SystemCreate Error : ") + fmodErrorMessage);
        }
        result = FMOD_System_Init(system,32,FMOD_INIT_NORMAL, NULL);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD SystemInit Error : ") + fmodErrorMessage);
        }
        nb++;
    }
    ~FMODSystemWrapper()
    {
        auto result=FMOD_System_Close(system);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD SystemClose Error : ") + fmodErrorMessage);
        }
        result = FMOD_System_Release(system);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD SystemRelease Error : ") + fmodErrorMessage);
        }
        nb--;
    }
    FMOD_SYSTEM* getSystem() {return system;}

};

unsigned FMODSystemWrapper::nb = 0;

struct FMODSingleton
{
private :
    std::shared_ptr<FMODSystemWrapper> f;
public :
    FMODSingleton() : f(nullptr) {}
    std::shared_ptr<FMODSystemWrapper> getSystem()
    {
        if(!f)
        {
            f.reset(new FMODSystemWrapper);
        }
        return f;
    }
};

static FMODSingleton GlobalFMODSystem;


FMOD_SYSTEM*Track::getSystem()
{
    return system.get()->getSystem();
}
Track::Track() : system(GlobalFMODSystem.getSystem()), music(nullptr),channel(nullptr),speed(1), hasSound(false), volume(1), repeat(false), muted(false), ABCFile(std::string()), paused(true)
{
}

Track::Track(const std::string& file)
    : system(GlobalFMODSystem.getSystem()), music(nullptr),channel(nullptr),speed(1), hasSound(false), volume(1), repeat(false), muted(false), ABCFile(std::string()), paused(true)
{
    setSound(file);
}

void Track::close()
{
    if(hasSound)
    {
        if(music)
        {
            auto result = FMOD_Sound_Release(music);
            if (result != FMOD_OK)
            {
                auto fmodErrorMessage = FMOD_ErrorString(result);
                throw std::runtime_error(std::string("FMOD soundRelease Error : ") + fmodErrorMessage);
            }
        }

        music = nullptr;
        hasSound=false;
    }
    if(ABCFile!="")
    {
        std::remove(ABCFile.c_str());
        ABCFile="";
    }
}
Track::~Track()
{
    close();
}

void Track::play()
{
    if(hasSound)
    {
        auto result = FMOD_Channel_SetPaused(channel,0);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD setPausedFalse Error : ") + fmodErrorMessage);
        }
        paused=false;
    }
}

void Track::pause()
{
    if(hasSound)
    {
        auto result = FMOD_Channel_SetPaused(channel,1);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD setPausedTrue Error : ") + fmodErrorMessage);
        }
        paused=true;
    }
}

void Track::setVolume(float vol)
{
    if(vol <0 || vol > 1)
        throw std::domain_error(toString(vol)+" is not a valid volume");
    if(hasSound)
    {
        auto result = FMOD_Channel_SetVolume(channel, vol);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD setVolume Error : ") + fmodErrorMessage);
        }
    }
    volume=vol;

}

float Track::getVolume() const
{
    return volume;
}

void Track::setSound(const std::string& file)
{
    close();
    std::string filename=file;
    if (filename.find(".abc")==file.length()-4)
    {
        convert(file.c_str());
        filename = file.substr(0, file.length()-4) + "1.mid";
        ABCFile = filename;
    }
	FMOD_CREATESOUNDEXINFO exinfo;
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.dlsname = "piano.dls";
    auto result = FMOD_System_CreateSound (getSystem(), filename.c_str(), FMOD_SOFTWARE | FMOD_LOOP_NORMAL, &exinfo, &music );
    if (result != FMOD_OK)
    {
        auto fmodErrorMessage = FMOD_ErrorString(result);
        throw std::runtime_error(std::string("FMOD createSound Error : ") + fmodErrorMessage);
    }
    speed = 1;
    loadChannel();
}


Track::operator bool() const
{
    return hasSound;
}

void Track::mute()
{
    if(hasSound)
    {
        auto result = FMOD_Channel_SetMute(channel, 1);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD mute Error : ") + fmodErrorMessage);
        }
    }
    muted=true;

}
void Track::unmute()
{
    if(hasSound)
    {
        auto result = FMOD_Channel_SetMute(channel, 0);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD unmute Error : ") + fmodErrorMessage);
        }
    }
    muted = false;

}

bool Track::isMute() const
{
    return muted;
}

float Track::getSpeed() const
{
    return speed;
}

unsigned SongPlayer::unique = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

SongPlayer::SongPlayer() : globalVolume(1), id(unique)
{
    unique++;
}

SongPlayer::SongPlayer(const Melody& m, const ChordProgression& c): globalVolume(1), id(unique)
{
    unique++;
    setSong(m, c);
}

SongPlayer::~SongPlayer()
{
    std::remove(("__melody"+toString(id)+".abc").c_str());
    std::remove(("__chord"+toString(id)+".abc").c_str());
}

#pragma GCC diagnostic pop


void SongPlayer::setSong(const Melody& m, const ChordProgression& c)
{
    if(m.size()!=c.size() && c.size()!=0)
        throw std::runtime_error("Melody and Chord Progression do not have same size");
    std::ofstream mf("__melody"+toString(id)+".abc");
    if(!(mf<<m))
        throw std::runtime_error("Unable to write Melody in file");
    if(c.size()!=0)
    {
        std::ofstream cf("__chord"+toString(id)+".abc");
        m.createABCChordFile(cf, c);
        mf.close();
        cf.close();
        chord.setSound("__chord"+toString(id)+".abc");
        chord.setVolume(globalVolume);
    }
    else
    {
        chord.close();
    }
    mf.close();
    originalBPM = m.m_BPM;
    melody.setSound("__melody"+toString(id)+".abc");
    melody.setVolume(globalVolume);

}

void SongPlayer::play()
{
    melody.play();
    chord.play();
}
void SongPlayer::pause()
{
    melody.pause();
    chord.pause();
}

void SongPlayer::setGlobalVolume(float vol)
{
    if(vol <0 || vol >1)
        throw std::domain_error("The volume is incorrect " + toString(vol));
    if(melody)
    {
        melody.setVolume(melody.getVolume()*vol/globalVolume);
        chord.setVolume(chord.getVolume()*vol/globalVolume);
    }
    globalVolume = vol;
}
void SongPlayer::setChordVolume(float vol)
{
    chord.setVolume(globalVolume * vol);
}
void SongPlayer::setMelodyVolume(float vol)
{
    melody.setVolume(globalVolume * vol);
}

float SongPlayer::getGlobalVolume() const
{
    return globalVolume;
}
float SongPlayer::getChordVolume() const
{
    return chord.getVolume()/globalVolume;
}
float SongPlayer::getMelodyVolume() const
{
    return melody.getVolume()/globalVolume;
}

void SongPlayer::mute()
{
    chord.mute();
    melody.mute();
}
void SongPlayer::muteChord()
{
    chord.mute();
}
void SongPlayer::muteMelody()
{
    melody.mute();
}

void SongPlayer::unmute()
{
    chord.unmute();
    melody.unmute();
}
void SongPlayer::unmuteChord()
{
    chord.unmute();
}
void SongPlayer::unmuteMelody()
{
    melody.unmute();
}

bool SongPlayer::isMute() const
{
    return melody.isMute() && chord.isMute();
}
bool SongPlayer::isChordMute() const
{
    return chord.isMute();
}
bool SongPlayer::isMelodyMute() const
{
    return melody.isMute();
}

void SongPlayer::setBPM(unsigned BPM)
{
    melody.setSpeed(static_cast<float>(BPM)/static_cast<float>(originalBPM));
    chord.setSpeed(static_cast<float>(BPM)/static_cast<float>(originalBPM));
}
unsigned SongPlayer::getBPM() const
{
    return static_cast<unsigned>(melody.getSpeed()*static_cast<float>(originalBPM));
}

void SongPlayer::setRepeat(bool r)
{
    chord.setRepeat(r);
    melody.setRepeat(r);
}
bool SongPlayer::getRepeat() const
{
    return melody.getRepeat();
}

void SongPlayer::setPosition(const Time& p)
{
    chord.setPosition(p);
    melody.setPosition(p);
}
Time SongPlayer::getPosition() const
{
    return melody.getPosition();
}
Time SongPlayer::getDuration() const
{
    return melody.getDuration();
}

void SongPlayer::stop()
{
    melody.stop();
    chord.stop();
}

//----------------TRICKY TO IMPLEMENT------------------------


Time Track::getDuration() const
{
    if(hasSound)
    {
        unsigned duration;
        auto result = FMOD_Sound_GetLength(music, &duration, FMOD_TIMEUNIT_MS);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD SoundGetLength Error : ") + fmodErrorMessage);
        }
        return Time(static_cast<float>(duration), Time::MS)/speed;
    }
    else
        return 0;
}



void Track::setSpeed(float acc)
{
    if(acc<=0)
        throw std::domain_error(toString(acc) + "is an invalid speed value");
    if(hasSound)
    {
        auto oldPos = getPosition();
        auto oldspeed = speed;
        auto result = FMOD_Sound_SetMusicSpeed(music, acc);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD setSpeed Error : ") + fmodErrorMessage);
        }
        setPosition(0);
        speed=acc;
        setPosition(oldPos*oldspeed/speed);
        result = FMOD_Channel_SetLoopPoints(channel, 0, FMOD_TIMEUNIT_MS,static_cast<unsigned>(getDuration().getNumber(Time::MS)), FMOD_TIMEUNIT_MS);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD setSpeed (setLoopPoints) Error : ") + fmodErrorMessage);
        }
    }
    else
        speed = acc;
}

void Track::setPosition(const Time& t)
{
    if(hasSound)
    {
        auto result = FMOD_Channel_SetPosition(channel, static_cast<unsigned>(t.getNumber(Time::MS)), FMOD_TIMEUNIT_MS);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD setPosition Error : ") + fmodErrorMessage);
        }
    }
}
Time Track::getPosition() const
{
    if(hasSound)
    {
        unsigned pos;
        auto result = FMOD_Channel_GetPosition(channel, &pos, FMOD_TIMEUNIT_MS);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD getPosition Error : ") + fmodErrorMessage);
        }
        return Time(static_cast<float>(pos), Time::MS);
    }
    else
        return Time(0);
}


void Track::setRepeat(bool rep)
{
    if(hasSound)
    {
        if(rep)
        {
            auto result = FMOD_Channel_SetLoopCount(channel, -1);
            if (result != FMOD_OK)
            {
                auto fmodErrorMessage = FMOD_ErrorString(result);
                throw std::runtime_error(std::string("FMOD setRepeatTrue Error : ") + fmodErrorMessage);
            }
        }
        else
        {
            auto result = FMOD_Channel_SetLoopCount(channel, 0);
            if (result != FMOD_OK)
            {
                auto fmodErrorMessage = FMOD_ErrorString(result);
                throw std::runtime_error(std::string("FMOD setRepeatFalse Error : ") + fmodErrorMessage);
            }
        }
    }
    repeat = rep;

}

bool Track::getRepeat() const
{
   return repeat;
}

void Track::stop()
{
    if(hasSound)
    {
        auto result = FMOD_Channel_Stop(channel);
        if (result != FMOD_OK)
        {
            auto fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error(std::string("FMOD Stop Error : ") + fmodErrorMessage);
        }
        loadChannel();
    }
}

void Track::loadChannel()
{
    auto result = FMOD_System_PlaySound(getSystem(), FMOD_CHANNEL_FREE, music, 1, &channel);
    if (result != FMOD_OK)
    {
        std::string fmodErrorMessage = FMOD_ErrorString(result);
        throw std::runtime_error("FMOD_PlaySOund Error : " +fmodErrorMessage);
    }
    hasSound = true;
    setVolume(volume);
    setRepeat(repeat);
    setSpeed(speed);
    if(muted)
        mute();
    else
        unmute();
    pause();
}

bool Track::isFinished()const
{
    if(hasSound && ! isPaused())
    {
        FMOD_BOOL isPlaying;
        auto result = FMOD_Channel_IsPlaying(channel, &isPlaying);
        if (result != FMOD_OK)
        {
            std::string fmodErrorMessage = FMOD_ErrorString(result);
            throw std::runtime_error("FMOD_IsPlaying Error : " +fmodErrorMessage);
        }
        return !isPlaying;
    }
    else
        return false;
}

bool Track::isPaused() const
{
    return paused;
}

bool SongPlayer::isFinished() const
{
    return melody.isFinished();
}

SongPlayer::operator bool() const
{
    return melody;
}

