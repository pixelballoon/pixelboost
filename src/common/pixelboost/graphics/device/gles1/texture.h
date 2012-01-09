#pragma once

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/texture.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1

#include "pixelboost/graphics/device/gles1/device.h"

namespace pixelboost
{

class TextureGLES1 : Texture
{
protected:
    TextureGLES1(GraphicsDeviceGLES1* _Device);
    virtual ~TextureGLES1();
    
public:
    void Load(const unsigned char* data, bool createMips, TextureFormat format);
    void Load(const std::string& image, bool createMips);
    void Bind(int unit);
    
private:
    GraphicsDeviceGLES1* _Device;
    GLuint _Texture;
    
    friend class GraphicsDeviceGLES1;
};

}

#endif
