#pragma once

#ifdef PIXELBOOST_PLATFORM_IOS

#include <map>
#include <string>

#include "pixelboost/audio/audioManagerSimple.h"

namespace pb
{

class AudioManagerSimpleIOS : public AudioManagerSimple
{
public:
    AudioManagerSimpleIOS();
    ~AudioManagerSimpleIOS();

public:
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
    int SfxGetId();
    void SfxPlay(Sound& sound);
    void SfxStop(Sound& sound);
    bool SfxIsPlaying(const Sound& sound);
    void SfxUpdateLooping(Sound& sound);
    void SfxUpdatePitch(Sound& sound);
    void SfxUpdateVolume(Sound& sound);
    
private:
    int _SoundId;
    std::map<int, void*> _Sounds;
    
    std::string _CurrentBgmName;
    bool _CurrentBgmLoop;
    bool _CurrentBgmVolume;
    
    bool _MuteBgm;
    bool _MuteSfx;
};
    
}

#endif
