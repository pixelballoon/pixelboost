#pragma once

#ifdef PIXELBOOST_BUILD_RELEASE
	#define PbLogDebug(system, format, ...) do {} while(0)
#else
	#ifdef PIXELBOOST_PLATFORM_ANDROID
		#include <android/log.h>
		#define PbLogDebug(system, format, ...) __android_log_print(ANDROID_LOG_DEBUG, system, format, ## __VA_ARGS__)
	#else
		#define PbLogDebug(system, format, ...) printf(format, ## __VA_ARGS__)
	#endif
#endif