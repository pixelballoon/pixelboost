#ifdef PIXELBOOST_PLATFORM_OSX

#include "pixelboost/audio/osx/audioManagerSimple.h"
#include "pixelboost/file/fileHelpers.h"

namespace pb
{
    
AudioManagerSimple* AudioManagerSimple::Instance()
{
    static AudioManagerSimpleOSX* instance = new AudioManagerSimpleOSX();
    return instance;
}
    
AudioManagerSimpleOSX::AudioManagerSimpleOSX()
{
    _CurrentBgmName = "";
    _MuteBgm = false;
    _MuteSfx = false;
}

AudioManagerSimpleOSX::~AudioManagerSimpleOSX()
{
    
}

void AudioManagerSimpleOSX::Update(float time)
{
    
}
    
bool AudioManagerSimpleOSX::IsBgmMuted()
{
    return _MuteBgm;
}
    
void AudioManagerSimpleOSX::MuteBgm(bool mute)
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
    
bool AudioManagerSimpleOSX::IsSfxMuted()
{
    return _MuteSfx;
}

void AudioManagerSimpleOSX::MuteSfx(bool mute)
{
    _MuteSfx = mute;
}

void AudioManagerSimpleOSX::LoadBgm(const std::string& name)
{
}

void AudioManagerSimpleOSX::LoadSfx(const std::string& name)
{
}

void AudioManagerSimpleOSX::PlayBgm(const std::string& name, bool loop, float volume)
{
    _CurrentBgmName = name;
    _CurrentBgmLoop = loop;
    _CurrentBgmVolume = volume;
    
    if (_MuteBgm || _CurrentBgmName == "")
        return;
    
     std::string fileName = "/data/audio/bgm/" + name + ".mp3";     
}
    
void AudioManagerSimpleOSX::StopBgm()
{
    _CurrentBgmName = "";
}

AudioManagerSimple::Sound AudioManagerSimpleOSX::PlaySfx(const std::string& name, float volume, float pitch)
{
    if (_MuteSfx)
        return Sound();
    
    std::string fileName = "/data/audio/sfx/" + name;
    
    return Sound();
}

}

#endif