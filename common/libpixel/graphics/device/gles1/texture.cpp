#include "libpixel/graphics/device/device.h"

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1

#include "libpixel/graphics/device/gles1/texture.h"

libpixel::TextureGLES1::TextureGLES1(GraphicsDeviceGLES1* device)
    : _Device(device)
{
    
}

libpixel::TextureGLES1::~TextureGLES1()
{
    glDeleteTextures(1, &_Texture);
}

void libpixel::TextureGLES1::Bind(int unit)
{
    _Device->BindTexture(this);
}

#endif
