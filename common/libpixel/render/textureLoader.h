#ifndef LIBPIXEL__COMMON__RENDER__SPRITELOADER__H
#define LIBPIXEL__COMMON__RENDER__SPRITELOADER__H

#include <string>

#include "libpixel/render/gl.h"

namespace libpixel
{

GLuint LoadTexture(void* image, bool createMips = false);
GLuint LoadTexture(const std::string& path, bool createMips = false);

}

#endif