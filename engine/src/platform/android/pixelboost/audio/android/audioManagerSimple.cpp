#ifdef PIXELBOOST_PLATFORM_ANDROID

#ifndef PIXELBOOST_DISABLE_SOUND

#include <vector>

#include "pixelboost/audio/android/audioManagerSimple.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/misc/jni.h"

using namespace pb;

AudioManagerSimple* AudioManagerSimple::Instance()
{
    static AudioManagerSimpleAndroid* instance = new AudioManagerSimpleAndroid();
    return instance;
}

AudioManagerSimpleAndroid::AudioManagerSimpleAndroid()
{
    _CurrentBgmName = "";
    _MuteBgm = false;
    _MuteSfx = false;
}

AudioManagerSimpleAndroid::~AudioManagerSimpleAndroid()
{
    
}

void AudioManagerSimpleAndroid::Update(float time)
{
    
}

bool AudioManagerSimpleAndroid::IsBgmMuted()
{
    return _MuteBgm;
}
    
void AudioManagerSimpleAndroid::MuteBgm(bool mute)
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

bool AudioManagerSimpleAndroid::IsSfxMuted()
{
    return _MuteSfx;
}

void AudioManagerSimpleAndroid::MuteSfx(bool mute)
{
    _MuteSfx = mute;
}

void AudioManagerSimpleAndroid::LoadBgm(const std::string& name)
{
    return;
}

void AudioManagerSimpleAndroid::LoadSfx(const std::string& name)
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

void AudioManagerSimpleAndroid::ReloadSfx()
{
    for (std::map<std::string, int>::iterator it = _Sfx.begin(); it != _Sfx.end(); ++it)
    {
        LoadSfx(it->first);
    }
}

void AudioManagerSimpleAndroid::PlayBgm(const std::string& name, bool loop, float volume)
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
    
void AudioManagerSimpleAndroid::StopBgm()
{
    _CurrentBgmName = "";

    JNIEnv* env = pb::Jni::GetJniEnv();
    
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "stopMusic", "()V");

    env->CallStaticVoidMethod(classId, methodId);
}

AudioManagerSimple::Sound AudioManagerSimpleAndroid::PlaySfx(const std::string& name, float volume, float pitch)
{
    if (_MuteSfx)
        return Sound(0);

    JNIEnv* env = pb::Jni::GetJniEnv();

    if (_Sfx.find(name) == _Sfx.end())
        return Sound(0);

    jint soundId = _Sfx[name];

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "playSound", "(IFF)I");
    jint result = env->CallStaticIntMethod(classId, methodId, (jint)soundId, (jfloat)volume, (jfloat)pitch);

    if (!result)
        return Sound(0);

    return Sound(result, name, volume, pitch);
}

void AudioManagerSimpleAndroid::SfxPlay(Sound& sound)
{
    sound = PlaySfx(sound.GetName(), sound.GetVolume(), sound.GetPitch());
}

void AudioManagerSimpleAndroid::SfxStop(Sound& sound)
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jint soundId = sound.GetId();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "stopSound", "(I)V");
    env->CallStaticVoidMethod(classId, methodId, (jint)soundId);
}

bool AudioManagerSimpleAndroid::SfxIsPlaying(const Sound& sound)
{
    PbLogWarn("pb.audio", "Checking if a sound is playing isn't possible on Android currently");
    
    return false;
}

void AudioManagerSimpleAndroid::SfxUpdateLooping(Sound& sound)
{
    PbLogWarn("pb.audio", "Note: Long samples may not loop on Android");

    JNIEnv* env = pb::Jni::GetJniEnv();
    
    jint soundId = sound.GetId();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "setSoundLooping", "(IZ)V");
    env->CallStaticVoidMethod(classId, methodId, (jint)soundId, (jboolean)sound.IsLooping());
}

void AudioManagerSimpleAndroid::SfxUpdatePitch(Sound& sound)
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jint soundId = sound.GetId();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "setSoundPitch", "(IF)V");
    env->CallStaticVoidMethod(classId, methodId, (jint)soundId, (jfloat)sound.GetPitch());
}

void AudioManagerSimpleAndroid::SfxUpdateVolume(Sound& sound)
{
    JNIEnv* env = pb::Jni::GetJniEnv();

    jint soundId = sound.GetId();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    jmethodID methodId = env->GetStaticMethodID(classId, "setSoundVolume", "(IF)V");
    env->CallStaticVoidMethod(classId, methodId, (jint)soundId, (jfloat)sound.GetVolume());
}

#endif

#endif
