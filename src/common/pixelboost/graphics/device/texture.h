#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/device.h"

namespace pb
{
    
class Texture
{
protected:
    Texture();
    virtual ~Texture();
    
public:
    enum TextureFormat
    {
        kTextureFormatRGBA,
    };
    
    virtual void LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format) = 0;
    virtual void LoadFromPng(const std::string& image, bool createMips);
    
    virtual void Bind(int unit = 0) = 0;
    
    const glm::vec2& GetSize();
    
protected:
    glm::vec2 _Size;
    
    friend class GraphicsDevice;
};

}

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1
#include "pixelboost/graphics/device/gles1/texture.h"
#endif

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2
#include "pixelboost/graphics/device/gles2/texture.h"
#endif

#ifdef PIXELBOOST_GRAPHICS_OPENGL2
#include "pixelboost/graphics/device/gl2/texture.h"
#endif
