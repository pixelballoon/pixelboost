#ifdef PIXELBOOST_PLATFORM_ANDROID

#ifndef PIXELBOOST_DISABLE_SOUND

#include <vector>

#include "pixelboost/audio/soundManager.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/misc/jni.h"

using namespace pb;

Sound::Sound(int id, const std::string& name, float volume, float pitch, bool looping)
    : _Id(id)
    , _Looping(false)
    , _Name(name)
    , _IsPlaying(false)
    , _Pitch(pitch)
    , _Volume(volume)
{
    
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
    /*
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
    */
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
    return;
}

void SoundManager::LoadSfx(const std::string& name)
{
    std::string fileName = "data/audio/sfx/" + name;

    JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(fileName.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");

    jmethodID methodId = env->GetStaticMethodID(classId, "loadSound", "(Ljava/lang/String;)I");
    jint result = env->CallStaticIntMethod(classId, methodId, jstr);

    env->DeleteLocalRef(jstr);

    if (!result)
        return;

    _Sfx[name] = result;
}

void SoundManager::PlayBgm(const std::string& name, bool loop, float volume)
{
    _CurrentBgmName = name;
    _CurrentBgmLoop = loop;
    _CurrentBgmVolume = volume;
    
    if (_MuteBgm || _CurrentBgmName == "")
        return;
    
    std::string fileName = "data/audio/bgm/" + name;
     
    JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(fileName.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "playMusic", "(Ljava/lang/String;F)V");

    env->CallStaticVoidMethod(classId, methodId, jstr, (jfloat)volume);
    env->DeleteLocalRef(jstr);
}
    
void SoundManager::StopBgm()
{
    _CurrentBgmName = "";

    JNIEnv* env = pb::Jni::GetJniEnv();
    
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "stopMusic", "()V");

    env->CallStaticVoidMethod(classId, methodId);
}

Sound SoundManager::PlaySfx(const std::string& name, float volume, float pitch)
{
    if (_MuteSfx)
        return Sound(0);

    JNIEnv* env = pb::Jni::GetJniEnv();

    if (_Sfx.find(name) == _Sfx.end())
        return Sound(0);

    jint soundId = _Sfx[name];

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "playSound", "(I)I");
    jint result = env->CallStaticIntMethod(classId, methodId, (jint)soundId);

    if (!result)
        return Sound(0);

    return Sound(result, name, volume, pitch);
}

void SoundManager::SfxPlay(Sound& sound)
{
    sound = PlaySfx(sound.GetName(), sound.GetVolume(), sound.GetPitch());
}

void SoundManager::SfxStop(Sound& sound)
{
    /*
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    [src stop];
    */
}

bool SoundManager::SfxIsPlaying(const Sound& sound)
{
    /*
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return false;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    return src.playing;
    */
}

void SoundManager::SfxUpdateLooping(Sound& sound)
{
    /*
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.looping = sound.IsLooping();
    */
}

void SoundManager::SfxUpdatePitch(Sound& sound)
{
    /*
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.pitch = sound.GetPitch();
    */
}

void SoundManager::SfxUpdateVolume(Sound& sound)
{
    /*
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.volume = sound.GetVolume();
    */
}

#endif

#endif
