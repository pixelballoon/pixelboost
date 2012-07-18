#ifdef PIXELBOOST_PLATFORM_IOS

#ifndef PIXELBOOST_DISABLE_SOUND

#include <vector>

#import "ObjectAL.h"
#import "ALSoundSource.h"

#include "pixelboost/audio/soundManager.h"
#include "pixelboost/file/fileHelpers.h"

using namespace pb;

Sound::Sound(const std::string& name, float volume, float pitch, bool looping)
    : _Looping(false)
    , _Name(name)
    , _IsPlaying(false)
    , _Pitch(pitch)
    , _Volume(volume)
{
    _Id = SoundManager::Instance()->SfxGetId();
}

int Sound::GetId() const
{
    return _Id;
}

void Sound::Play()
{
    _IsPlaying = true;
    SoundManager::Instance()->SfxPlay(*this);
}

void Sound::Stop()
{
    _IsPlaying = false;
    SoundManager::Instance()->SfxStop(*this);
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
    SoundManager::Instance()->SfxUpdateLooping(*this);
}

float Sound::GetPitch() const
{
    return _Pitch;
}

void Sound::SetPitch(float pitch)
{
    _Pitch = pitch;
    SoundManager::Instance()->SfxUpdatePitch(*this);
}

float Sound::GetVolume() const
{
    return _Volume;
}

void Sound::SetVolume(float volume)
{
    _Volume = volume;
    SoundManager::Instance()->SfxUpdateVolume(*this);
}

SoundManager::SoundManager()
{
    _CurrentBgmName = "";
    _MuteBgm = false;
    _MuteSfx = false;
    
    [OALSimpleAudio sharedInstance].allowIpod = NO;
    [OALSimpleAudio sharedInstance].honorSilentSwitch = YES;
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
    std::vector<int> inactiveSounds;
    
    for (std::map<int, void*>::iterator it = _Sounds.begin(); it != _Sounds.end(); ++it)
    {
        id<ALSoundSource> src = (id<ALSoundSource>)it->second;
        
        if (src.playing == false)
            inactiveSounds.push_back(it->first);
    }
    
    for (std::vector<int>::iterator it = inactiveSounds.begin(); it != inactiveSounds.end(); ++it)
    {
        _Sounds.erase(*it);
    }
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
    std::string fileName = FileHelpers::GetRootPath() + "/data/audio/sfx/" + name;
    
    [[OALSimpleAudio sharedInstance] preloadEffect:[NSString stringWithUTF8String:fileName.c_str()]];
}

void SoundManager::PlayBgm(const std::string& name, bool loop, float volume)
{
    _CurrentBgmName = name;
    _CurrentBgmLoop = loop;
    _CurrentBgmVolume = volume;
    
    if (_MuteBgm || _CurrentBgmName == "")
        return;
    
     std::string fileName = FileHelpers::GetRootPath() + "/data/audio/bgm/" + name;
     
    [[OALSimpleAudio sharedInstance] playBg:[NSString stringWithUTF8String:fileName.c_str()] volume:volume pan:0.f loop:loop];
}
    
void SoundManager::StopBgm()
{
    _CurrentBgmName = "";
    
    [[OALSimpleAudio sharedInstance] stopBg];
}

Sound SoundManager::PlaySfx(const std::string& name, float volume, float pitch)
{
    if (_MuteSfx)
        return Sound();
    
    Sound sound(name, volume, pitch);
    
    sound.Play();
    
    return sound;
}

int SoundManager::SfxGetId()
{
    return _SoundId++;
}

void SoundManager::SfxPlay(const Sound& sound)
{
    if (_MuteSfx)
        return;
    
    std::string filename = FileHelpers::GetRootPath() + "/data/audio/sfx/" + sound.GetName();
    
    id<ALSoundSource> instance = [[OALSimpleAudio sharedInstance] playEffect:[NSString stringWithUTF8String:filename.c_str()] volume:sound.GetVolume() pitch:sound.GetPitch() pan:0.f loop:sound.IsLooping()];
    [instance retain];
    _Sounds[sound.GetId()] = instance;
}

void SoundManager::SfxStop(const Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    [src stop];
}

bool SoundManager::SfxIsPlaying(const Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return false;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    return src.playing;
}

void SoundManager::SfxUpdateLooping(const Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.looping = sound.IsLooping();
}

void SoundManager::SfxUpdatePitch(const Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.pitch = sound.GetPitch();
}

void SoundManager::SfxUpdateVolume(const Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.volume = sound.GetVolume();
}

#endif

#endif
