#pragma once

#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/texture.h"

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1

#include "libpixel/graphics/device/gles1/device.h"

namespace libpixel
{

class TextureGLES1 : Texture
{
protected:
    TextureGLES1();
    
public:
    virtual ~TextureGLES1();
    
    void Load(const std::string& image, bool createMips);
    void Bind(int unit);
    
private:
    GLuint _Texture;
    
    friend class GraphicsDeviceGLES1;
};

}

#endif
