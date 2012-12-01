#pragma once

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/texture.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL

#include "pixelboost/graphics/device/gl/device.h"

namespace pb
{
    
class GraphicsDeviceGL;

class TextureGL : public Texture
{
protected:
    TextureGL(GraphicsDeviceGL* device);
    virtual ~TextureGL();
    
public:
    bool LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format, bool hasPremultipliedAlpha);
    
protected:
    void Bind(int textureUnit);
    
private:
    GraphicsDeviceGL* _Device;
    GLuint _Texture;
    
    friend class GraphicsDeviceGL;
};

}

#endif
