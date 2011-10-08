#ifndef LIBPIXEL__COMMON__GRAPHICS__DEVICE__TEXTURE__H
#define LIBPIXEL__COMMON__GRAPHICS__DEVICE__TEXTURE__H

#include <string>

#include "libpixel/graphics/device/device.h"

namespace libpixel
{

GLuint LoadTexture(void* image, bool createMips = false);
GLuint LoadTexture(const std::string& path, bool createMips = false);

}

#endif