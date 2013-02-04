#pragma once

#ifdef PIXELBOOST_PLATFORM_ANDROID

#include <map>
#include <string>

#include "pixelboost/audio/audioManagerSimple.h"

namespace pb
{
    
class AudioManagerSimpleAndroid : public AudioManagerSimple
{
public:
    AudioManagerSimpleAndroid();
    ~AudioManagerSimpleAndroid();

    void Update(float time);
    
    bool IsBgmMuted();
    void MuteBgm(bool mute=true);
    bool IsSfxMuted();
    void MuteSfx(bool mute=true);

    void LoadBgm(const std::string& name);
    void LoadSfx(const std::string& name);

    void ReloadSfx();
    
    void PlayBgm(const std::string& name, bool loop, float volume=1.f);
    void StopBgm();
    
    Sound PlaySfx(const std::string& name, float volume=1.f, float pitch=1.f);
    
protected:
    void SfxPlay(Sound& sound);
    void SfxStop(Sound& sound);
    bool SfxIsPlaying(const Sound& sound);
    void SfxUpdateLooping(Sound& sound);
    void SfxUpdatePitch(Sound& sound);
    void SfxUpdateVolume(Sound& sound);
    
private:
    int _SoundId;
    std::map<int, void*> _Sounds;

    std::map<std::string, int> _Sfx;
    std::map<std::string, int> _Bgm;
    
    std::string _CurrentBgmName;
    bool _CurrentBgmLoop;
    bool _CurrentBgmVolume;
    
    bool _MuteBgm;
    bool _MuteSfx;
};
    
}

#endif
