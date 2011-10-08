#ifndef LIBPIXEL__GRAPHICS__DEVICE__DEVICE__H
#define LIBPIXEL__GRAPHICS__DEVICE__DEVICE__H

#ifdef __APPLE__
    #include "TargetConditionals.h"
    
    #if TARGET_OS_IPHONE
        #include <OpenGLES/ES1/gl.h>
        #include <OpenGLES/ES1/glext.h>
    #else
        #include <OpenGL/gl.h>
    #endif
#else
	#include <GLES/gl.h>
#endif

#endif
