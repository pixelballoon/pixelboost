#pragma once

#if defined(PIXELBOOST_BUILD_RELEASE)
	#define PbLogDebug(system, format, ...) do {} while(0)
    #define PbLogError(system, format, ...) do {} while(0)
    #define PbLogWarn(system, format, ...) do {} while(0)
#elif defined(PIXELBOOST_PLATFORM_ANDROID)
	#include <android/log.h>
	#define PbLogDebug(system, format, ...) __android_log_print(ANDROID_LOG_DEBUG, system, format, ## __VA_ARGS__)
    #define PbLogError(system, format, ...) __android_log_print(ANDROID_LOG_ERROR, system, format, ## __VA_ARGS__)
    #define PbLogWarn(system, format, ...) __android_log_print(ANDROID_LOG_WARN, system, format, ## __VA_ARGS__)
#elif defined(PIXELBOOST_PLATFORM_NACL)
    #include "ppapi/cpp/instance.h"
    #include "ppapi/cpp/var.h"
    #include "pixelboost/framework/engine.h"

	#define PbLogDebug(system, format, ...) { char status[1024]; snprintf(status, 1024, format, ## __VA_ARGS__); static_cast<pp::Instance*>(pb::Engine::Instance()->GetViewController())->PostMessage(pp::Var(status)); }
    #define PbLogError(system, format, ...) { char status[1024]; snprintf(status, 1024, format, ## __VA_ARGS__); static_cast<pp::Instance*>(pb::Engine::Instance()->GetViewController())->PostMessage(pp::Var(status)); }
    #define PbLogWarn(system, format, ...) { char status[1024]; snprintf(status, 1024, format, ## __VA_ARGS__); static_cast<pp::Instance*>(pb::Engine::Instance()->GetViewController())->PostMessage(pp::Var(status)); }
#else
    #define PbLogDebug(system, format, ...) printf(format, ## __VA_ARGS__)
	#define PbLogError(system, format, ...) printf(format, ## __VA_ARGS__)
    #define PbLogWarn(system, format, ...) printf(format, ## __VA_ARGS__)
#endif
