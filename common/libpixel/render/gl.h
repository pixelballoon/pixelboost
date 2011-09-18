#ifndef LIBPIXEL__RENDER__GL__H
#define LIBPIXEL__RENDER__GL__H

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
