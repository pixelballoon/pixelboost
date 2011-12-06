#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1

#include "pixelboost/graphics/device/gles1/texture.h"

pixelboost::TextureGLES1::TextureGLES1(GraphicsDeviceGLES1* device)
    : _Device(device)
{
    
}

pixelboost::TextureGLES1::~TextureGLES1()
{
    glDeleteTextures(1, &_Texture);
}

void pixelboost::TextureGLES1::Bind(int unit)
{
    _Device->BindTexture(this);
}

#endif
