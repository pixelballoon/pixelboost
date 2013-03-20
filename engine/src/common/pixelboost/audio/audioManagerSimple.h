#pragma once

#include <map>
#include <string>

namespace pb
{
    
class AudioManagerSimple
{
protected:
    AudioManagerSimple();
    virtual ~AudioManagerSimple();
    
public:
    static AudioManagerSimple* Instance();

public:
    class Sound
    {
    public:
        Sound(int id=0, const std::string& name="", float volume=1.f, float pitch=1.f, bool looping=false);
        
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
    };

public:    
    virtual void Update(float time);
    
    virtual bool IsBgmMuted();
    virtual void MuteBgm(bool mute=true);
    virtual bool IsSfxMuted();
    virtual void MuteSfx(bool mute=true);

    virtual void LoadBgm(const std::string& name);
    virtual void LoadSfx(const std::string& name);

    virtual void ReloadSfx();
    
    virtual void PlayBgm(const std::string& name, bool loop, float volume=1.f);
    virtual void StopBgm();
    
    virtual Sound PlaySfx(const std::string& name, float volume=1.f, float pitch=1.f);
    
protected:
    virtual void SfxPlay(Sound& sound);
    virtual void SfxStop(Sound& sound);
    virtual bool SfxIsPlaying(const Sound& sound);
    virtual void SfxUpdateLooping(Sound& sound);
    virtual void SfxUpdatePitch(Sound& sound);
    virtual void SfxUpdateVolume(Sound& sound);
};
    
}
