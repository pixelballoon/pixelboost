#ifdef PIXELBOOST_PLATFORM_OSX

#include "pixelboost/audio/soundManager.h"
#include "pixelboost/file/fileHelpers.h"

namespace pb
{

SoundManager::SoundManager()
{
    _CurrentBgmName = "";
    _MuteBgm = false;
    _MuteSfx = false;
}

SoundManager::~SoundManager()
{
    
}
    
SoundManager* SoundManager::Instance()
{
    static SoundManager* instance = new SoundManager();
    return instance;
}
    
void SoundManager::Update(float time)
{
    
}
    
void SoundManager::MuteBgm(bool mute)
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

void SoundManager::MuteSfx(bool mute)
{
    _MuteSfx = mute;
}

void SoundManager::LoadBgm(const std::string& name)
{
}

void SoundManager::LoadSfx(const std::string& name, bool compressed)
{
}

void SoundManager::PlayBgm(const std::string& name, bool loop, float volume)
{
    _CurrentBgmName = name;
    _CurrentBgmLoop = loop;
    _CurrentBgmVolume = volume;
    
    if (_MuteBgm || _CurrentBgmName == "")
        return;
    
     std::string fileName = "/data/audio/bgm/" + name + ".mp3";     
}
    
void SoundManager::StopBgm()
{
    _CurrentBgmName = "";
}

void SoundManager::PlaySfx(const std::string& name, bool compressed, float volume, float pitch)
{
    if (_MuteSfx)
        return;
    
    std::string fileName = "/data/audio/sfx/" + name + (compressed ? ".mp3" : ".wav");
}

}

#endif