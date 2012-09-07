#ifdef PIXELBOOST_PLATFORM_OSX

#include "pixelboost/audio/soundManager.h"
#include "pixelboost/file/fileHelpers.h"

namespace pb
{
    
Sound::Sound(const std::string& name, float volume, float pitch, bool looping)
{
    
}

int Sound::GetId() const
{
    return 0;
}

void Sound::Play()
{
    _IsPlaying = true;
}
    
void Sound::Stop()
{
    _IsPlaying = false;
}

bool Sound::IsPlaying() const
{
    return _IsPlaying;
}

const std::string& Sound::GetName() const
{
    return _Name;
}

bool Sound::IsLooping() const
{
    return _Looping;
}
    
void Sound::SetLooping(bool looping)
{
    _Looping = looping;
}

float Sound::GetPitch() const
{
    return _Pitch;
}
    
void Sound::SetPitch(float pitch)
{
    _Pitch = pitch;
}

float Sound::GetVolume() const
{
    return _Volume;
}
    
void Sound::SetVolume(float volume)
{
    _Volume = volume;
}

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
    
bool SoundManager::IsBgmMuted()
{
    return _MuteBgm;
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
    
bool SoundManager::IsSfxMuted()
{
    return _MuteSfx;
}

void SoundManager::MuteSfx(bool mute)
{
    _MuteSfx = mute;
}

void SoundManager::LoadBgm(const std::string& name)
{
}

void SoundManager::LoadSfx(const std::string& name)
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

Sound SoundManager::PlaySfx(const std::string& name, float volume, float pitch)
{
    if (_MuteSfx)
        return Sound();
    
    std::string fileName = "/data/audio/sfx/" + name;
    
    return Sound();
}

}

#endif