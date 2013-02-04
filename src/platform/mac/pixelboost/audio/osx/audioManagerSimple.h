#pragma once

#ifdef PIXELBOOST_PLATFORM_OSX

#include <string>

#include "pixelboost/audio/audioManagerSimple.h"

namespace pb
{
    
class AudioManagerSimpleOSX : public AudioManagerSimple
{
public:
    AudioManagerSimpleOSX();
    ~AudioManagerSimpleOSX();
    
    void Update(float time);
    
    bool IsBgmMuted();
    void MuteBgm(bool mute=true);
    bool IsSfxMuted();
    void MuteSfx(bool mute=true);
    
    void LoadBgm(const std::string& name);
    void LoadSfx(const std::string& name);
    
    void PlayBgm(const std::string& name, bool loop, float volume=1.f);
    void StopBgm();
    
    Sound PlaySfx(const std::string& name, float volume=1.f, float pitch=1.f);
    
protected:
    std::string _CurrentBgmName;
    bool _CurrentBgmLoop;
    bool _CurrentBgmVolume;
    
    bool _MuteBgm;
    bool _MuteSfx;
};
    
}

#endif