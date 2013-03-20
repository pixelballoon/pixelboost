#ifdef PIXELBOOST_PLATFORM_ANDROID

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/misc/jni.h"

using namespace pb;

std::string pb::FileHelpers::GetBundlePath()
{
    JNIEnv* env = Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");

    jmethodID methodId = env->GetStaticMethodID(classId, "getBundleFilePath", "()Ljava/lang/String;");
    jstring result = static_cast<jstring>(env->CallStaticObjectMethod(classId, methodId));

    const char *nativeString = env->GetStringUTFChars(result, 0);
    std::string savePath = nativeString;
    env->ReleaseStringUTFChars(result, nativeString);

    return savePath;
}
    
std::string pb::FileHelpers::GetSavePath()
{
    JNIEnv* env = Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");
    
    jmethodID methodId = env->GetStaticMethodID(classId, "getSaveFilePath", "()Ljava/lang/String;");
    jstring result = static_cast<jstring>(env->CallStaticObjectMethod(classId, methodId));

    const char *nativeString = env->GetStringUTFChars(result, 0);
    std::string savePath = nativeString;
    env->ReleaseStringUTFChars(result, nativeString);

    return savePath;
}

#endif
