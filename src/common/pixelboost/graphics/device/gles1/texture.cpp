#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1

#include "pixelboost/graphics/device/gles1/texture.h"

pb::TextureGLES1::TextureGLES1(GraphicsDeviceGLES1* device)
    : _Device(device)
{
    
}

pb::TextureGLES1::~TextureGLES1()
{
    glDeleteTextures(1, &_Texture);
}

void pb::TextureGLES1::Bind(int unit)
{
    _Device->BindTexture(this);
}

#endif
