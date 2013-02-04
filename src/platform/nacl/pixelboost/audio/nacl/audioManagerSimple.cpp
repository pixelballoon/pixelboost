#ifdef PIXELBOOST_PLATFORM_NACL

#include "pixelboost/audio/soundManager.h"
#include "pixelboost/file/fileHelpers.h"

namespace pb
{

AudioManagerSimple::AudioManagerSimple()
{
    _CurrentBgmName = "";
    _MuteBgm = false;
    _MuteSfx = false;
}

AudioManagerSimple::~AudioManagerSimple()
{
    
}
    
AudioManagerSimple* AudioManagerSimple::Instance()
{
    static AudioManagerSimple* instance = new AudioManagerSimple();
    return instance;
}
    
void AudioManagerSimple::Update(float time)
{
    
}
    
void AudioManagerSimple::MuteBgm(bool mute)
{
    _MuteBgm = mute;
    
    if (_MuteBgm)
    {
        std::string currentBgm = _CurrentBgmName;
        StopBgm();
        _CurrentBgmName = currentBgm;
    } else {
        PlayBgm(_CurrentBgmName, _CurrentBgmLoop, _CurrentBgmVolume);
    }
}

void AudioManagerSimple::MuteSfx(bool mute)
{
    _MuteSfx = mute;
}

void AudioManagerSimple::LoadBgm(const std::string& name)
{
}

void AudioManagerSimple::LoadSfx(const std::string& name, bool compressed)
{
}

void AudioManagerSimple::PlayBgm(const std::string& name, bool loop, float volume)
{
    _CurrentBgmName = name;
    _CurrentBgmLoop = loop;
    _CurrentBgmVolume = volume;
    
    if (_MuteBgm || _CurrentBgmName == "")
        return;
    
     std::string fileName = "/data/audio/bgm/" + name + ".mp3";     
}
    
void AudioManagerSimple::StopBgm()
{
    _CurrentBgmName = "";
}

void AudioManagerSimple::PlaySfx(const std::string& name, bool compressed, float volume, float pitch)
{
    if (_MuteSfx)
        return;
    
    std::string fileName = "/data/audio/sfx/" + name + (compressed ? ".mp3" : ".wav");
}

}

#endif
