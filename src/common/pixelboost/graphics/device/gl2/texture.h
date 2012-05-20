#pragma once

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/texture.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL2

#include "pixelboost/graphics/device/gles1/device.h"

namespace pb
{

class TextureGL2 : public Texture
{
protected:
    TextureGL2(GraphicsDeviceGL2* device);
    virtual ~TextureGL2();
    
public:
    void LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format);
    
public:
    void Bind(int textureUnit);
    
private:
    GraphicsDeviceGL2* _Device;
    GLuint _Texture;
    
    friend class GraphicsDeviceGL2;
};

}

#endif
