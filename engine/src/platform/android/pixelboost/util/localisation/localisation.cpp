#ifdef PIXELBOOST_PLATFORM_ANDROID

#include "pixelboost/misc/jni.h"
#include "pixelboost/util/localisation/localisation.h"

std::string pb::GetCurrentLocale()
{
	JNIEnv* env = pb::Jni::GetJniEnv();

    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");

	jmethodID methodId = env->GetStaticMethodID(classId, "getCurrentLocale", "()Ljava/lang/String;");
    jstring result = static_cast<jstring>(env->CallStaticObjectMethod(classId, methodId));

    const char *nativeString = env->GetStringUTFChars(result, 0);
    std::string currentLocale = nativeString;
    env->ReleaseStringUTFChars(result, nativeString);

    return currentLocale;
}

#endif
