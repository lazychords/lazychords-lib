#ifndef TRACK_HPP_INCLUDED
#define TRACK_HPP_INCLUDED

#include <string>
#include "Music.hpp"
#include "Utilities.hpp"
#include <memory>
extern "C"
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wignored-qualifiers"
    #include "fmodex/fmod.h"
    #pragma GCC diagnostic pop
}

struct FMODSystemWrapper;

class Track
{
public :
    /**
     *@brief Initializes the track from the sound described in the filename
     *The sound can be abc, midi, mp3
     *The volume is at max, repeat is set to false, speed is set to 1 the sound is in pause, and mute is set to false
     *@param Filename is the location of the sound
     *@throw std::runtime_error("Unable to load file")
    **/
    Track(const std::string& filename);

    /**
     *@brief Creates an invalid Track
     *The volume is at max, repeat is set to false, speed i set to 1 and mute is set to false
    **/
    Track();
    Track(const Track&) = delete;
    Track& operator=(const Track&) = delete;

    /**
     *@brief If the Track is valid, plays the sound
    **/
    void play();

    /**
     *@brief If the Track is valid, pauses the sound
    **/
    void pause();

    /**
     *@brief If the track is valid, returns the duration of the sound at the current speed, otherwise, returns 0
     *@return The duration of the sound at the current speed if the track is valid, otherwise 0
    **/
    Time getDuration() const __attribute__((pure));


    /**
     *@brief Sets the Volume of the sound (or sets the volume for future use).
     *This function does not unmute if it is mute
     *@param vol is the new Volume. The Volume range is [0,1]
     *@throw std::domain_error("%f is an invalid volume")
    **/
    void setVolume(float vol);

    /**
     *@brief Returns the volume of the sound independently of its validity
     *The value does not depend on the "mute" state
     *@return Returns the volume of the sound.
    **/
    float getVolume() const __attribute__((pure));

    /**
     *@brief Sets the Track to a new Sound
     *The speed is reset to the speed of the new Sound, the volume, the repeat and mute options are kept unchanged
     *@param Filename is the location of the sound
     *@throw std::runtime_error("Unable to load file")
    **/
    void setSound(const std::string& filename);

    /**
     *@return if the track is valid
    **/
    operator bool () const __attribute__((pure));

    /**
     *@brief Mutes the sound
    **/
    void mute();

    /**
     *@brief Unmutes the Sound
    **/
    void unmute();

    /**
     *@brief return whether the sound is muted (independently of its validity)
    **/
    bool isMute() const __attribute__((pure)) ;

    /**
     *@brief Sets the speed.  A speed of 2 corresponds to twice the normal speed
     *@param s is the new speed
     *@throw std::domain_error("%f is an invalid speed") if speed <0
    **/
    void setSpeed(float s);

    /**
     *@brief returns the speed
    **/
    float getSpeed() const __attribute__((pure));

    /**
     *@brief Closes the sound (The Track becomes invalid)
    **/
    void close();

    /**
     *@brief Set the repeat option independently of the validity of the track
    **/
    void setRepeat(bool repeat);

    /**
     *@brief Return the repeat state independently of the validity of the track
    **/
    bool getRepeat() const __attribute__((pure));

    /**
     *@brief If the track is valid, sets the position in the track, otherwise it does nothing
    **/
    void setPosition(const Time& t);

    /**
     *@brief Returns the position in the Track (taking the speed into account) if the Track is valid, 0 otherwise
    **/
    Time getPosition() const __attribute__((pure));

    /**
     *@brief The Music stops when the object is destroyed
    **/
    ~Track();

    void stop();

    bool isFinished() const __attribute__((pure));
    bool isPaused() const __attribute__((pure));
private :
    std::shared_ptr<FMODSystemWrapper> system;///< state of fmod
    FMOD_SOUND *music; ///< represents the music stream
    FMOD_CHANNEL *channel; ///< represent the channel used to play the music
    FMOD_SYSTEM*getSystem() __attribute__((pure));
    float speed;
    bool hasSound;
    float volume;
    bool repeat;
    bool muted;
    std::string ABCFile;
    void loadChannel();
    bool paused;
};

class SongPlayer
{
public :
    SongPlayer();
    SongPlayer(const Melody& m, const ChordProgression& c);
    SongPlayer(const SongPlayer& o) = delete;
    SongPlayer& operator=(const SongPlayer& o) = delete;

    void setSong(const Melody& m, const ChordProgression& c = ChordProgression());

    void play();
    void pause();

    void setGlobalVolume(float vol);
    void setChordVolume(float vol);
    void setMelodyVolume(float vol);

    float getGlobalVolume() const __attribute__((pure));
    float getChordVolume() const __attribute__((pure));
    float getMelodyVolume() const __attribute__((pure));

    void mute();
    void muteChord();
    void muteMelody();
    void unmute();
    void unmuteChord();
    void unmuteMelody();

    bool isMute()const __attribute__((pure));
    bool isChordMute() const __attribute__((pure));
    bool isMelodyMute() const __attribute__((pure));

    void setBPM(unsigned BPM);
    unsigned getBPM() const __attribute__((pure));

    void setPosition(const Time& p);
    Time getPosition() const __attribute__((pure));
    Time getDuration() const __attribute__((pure));

    void setRepeat(bool r);
    bool getRepeat() const __attribute__((pure));
    void stop();
    operator bool() const __attribute__((pure));
    bool isFinished() const __attribute__((pure));
    ~SongPlayer();
private :
    static unsigned unique;
    Track chord;
    Track melody;
    float globalVolume;
    unsigned originalBPM;
    unsigned id;
};


#endif // TRACK_HPP_INCLUDED
