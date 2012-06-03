#pragma once

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/texture.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2

namespace pb
{
    
class GraphicsDeviceGLES2;

class TextureGLES2 : public Texture
{
protected:
    TextureGLES2(GraphicsDeviceGLES2* device);
    virtual ~TextureGLES2();
    
public:
    void LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format);
    
protected:
    void Bind(int textureUnit);
    
private:
    GraphicsDeviceGLES2* _Device;
    GLuint _Texture;
    
    friend class GraphicsDeviceGLES2;
};

}

#endif
