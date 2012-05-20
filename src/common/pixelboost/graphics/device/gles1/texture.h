#pragma once

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/texture.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1

#include "pixelboost/graphics/device/gles1/device.h"

namespace pb
{

class TextureGLES1 : public Texture
{
protected:
    TextureGLES1(GraphicsDeviceGLES1* _Device);
    virtual ~TextureGLES1();
    
public:
    void LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format);
    void LoadFromPng(const std::string& image, bool createMips);
    
public:
    void Bind(int textureUnit);
    
private:
    GraphicsDeviceGLES1* _Device;
    GLuint _Texture;
    
    friend class GraphicsDeviceGLES1;
};

}

#endif
