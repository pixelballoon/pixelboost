#pragma once

#ifdef PIXELBOOST_PLATFORM_OSX

#include <string>

namespace pb
{
    
class Sound
{
public:
    Sound(const std::string& name="", float volume=1.f, float pitch=1.f, bool looping=false);
    
    int GetId() const;
    
    void Play();
    void Stop();
    bool IsPlaying() const;
    
    const std::string& GetName() const;
    
    bool IsLooping() const;
    void SetLooping(bool looping);
    
    float GetPitch() const;
    void SetPitch(float pitch);
    
    float GetVolume() const;
    void SetVolume(float volume);
    
private:
    int _Id;
    bool _IsPlaying;
    bool _Looping;
    std::string _Name;
    float _Pitch;
    float _Volume;
    
    friend class SoundManager;
};

class SoundManager
{
private:
    SoundManager();
    ~SoundManager();
    
public:
    static SoundManager* Instance();
    
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
    
private:
    std::string _CurrentBgmName;
    bool _CurrentBgmLoop;
    bool _CurrentBgmVolume;
    
    bool _MuteBgm;
    bool _MuteSfx;
};
    
}

#endif