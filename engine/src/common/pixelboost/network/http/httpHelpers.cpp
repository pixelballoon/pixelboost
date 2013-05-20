#include "pixelboost/network/http/httpHelpers.h"

#ifdef PIXELBOOST_PLATFORM_ANDROID
#include "pixelboost/misc/jni.h"
#endif

#ifdef PIXELBOOST_PLATFORM_IOS
#import <UIKit/UIKit.h>
#endif

#ifdef PIXELBOOST_PLATFORM_OSX
#import <AppKit/AppKit.h>
#endif

using namespace pb;

void HttpHelpers::OpenUrl(const std::string& url)
{
#ifdef PIXELBOOST_PLATFORM_IOS
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
#endif
    
#ifdef PIXELBOOST_PLATFORM_OSX
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
#endif

#ifdef PIXELBOOST_PLATFORM_ANDROID
	JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(fileName.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");

    jmethodID methodId = env->GetStaticMethodID(classId, "openUrl", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(classId, methodId, jstr);

    env->DeleteLocalRef(jstr);
#endif
}
