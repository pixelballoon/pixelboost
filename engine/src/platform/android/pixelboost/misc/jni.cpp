#ifdef PIXELBOOST_PLATFORM_ANDROID

#include <jni.h>

#include "pixelboost/misc/jni.h"

using namespace pb;

JavaVM* g_JavaVM;

JNIEnv* Jni::GetJniEnv()
{
	JNIEnv* env;
	g_JavaVM->AttachCurrentThread(&env, 0);
	return env;
}

#endif
