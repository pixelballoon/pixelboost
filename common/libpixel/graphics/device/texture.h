#pragma once

#include <string>

#include "libpixel/graphics/device/device.h"

namespace libpixel
{

GLuint LoadTexture(void* image, bool createMips = false);
GLuint LoadTexture(const std::string& path, bool createMips = false);
    
class Texture
{
protected:
    Texture();
    virtual ~Texture();
    
public:
    virtual void Load(const std::string& image, bool createMips) = 0;
    virtual void Bind(int unit = 0) = 0;
    
    friend class GraphicsDevice;
};

}

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1
#include "libpixel/graphics/device/gles1/texture.h"
#endif
