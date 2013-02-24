#pragma once

#include <string>

#if defined(PIXELBOOST_BUILD_MASTER)
	#define PlLogDebug(system, format, ...) do {} while(0)
    #define PlLogError(system, format, ...) do {} while(0)
    #define PlLogInfo(system, format, ...) do {} while(0)
    #define PlLogWarn(system, format, ...) do {} while(0)
#else
    #define PlLogDebug(system, format, ...) do { printf("[Debug] (%s) ", system); printf(format, ## __VA_ARGS__); printf("\n"); } while(false)
	#define PlLogError(system, format, ...) do { printf("[Error] (%s) ", system); printf(format, ## __VA_ARGS__); printf("\n"); } while(false)
	#define PlLogInfo(system, format, ...) do { printf(" [Info] (%s) ", system); printf(format, ## __VA_ARGS__); printf("\n"); } while(false)
    #define PlLogWarn(system, format, ...) do { printf(" [Warn] (%s) ", system); printf(format, ## __VA_ARGS__); printf("\n"); } while(false)
#endif
