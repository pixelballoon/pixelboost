#pragma once

#ifdef PIXELBOOST_PLATFORM_WINDOWS

#include <string>

namespace pb
{

class AudioManagerSimple
{
private:
    AudioManagerSimple();
    ~AudioManagerSimple();
    
public:
    static AudioManagerSimple* Instance();
    
    void Update(float time);
    
    void MuteBgm(bool mute=true);
    void MuteSfx(bool mute=true);
    
    void LoadBgm(const std::string& name);
    void LoadSfx(const std::string& name, bool compressed);
    
    void PlayBgm(const std::string& name, bool loop, float volume=1.f);
    void StopBgm();
    
    void PlaySfx(const std::string& name, bool compressed, float volume=1.f, float pitch=1.f);
    
private:
    std::string _CurrentBgmName;
    bool _CurrentBgmLoop;
    bool _CurrentBgmVolume;
    
    bool _MuteBgm;
    bool _MuteSfx;
};
    
}

#endif
