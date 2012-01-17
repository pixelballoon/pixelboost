#pragma once

#include <string>

#include "pixelboost/graphics/device/device.h"

namespace pixelboost
{
    
class Texture
{
protected:
    Texture();
    virtual ~Texture();
    
public:
    enum TextureFormat
    {
        kTextureFormatUncompressed
    };
    
    virtual void Load(const unsigned char* data, bool createMips, TextureFormat format) = 0;
    virtual void Load(const std::string& image, bool createMips) = 0;
    
    virtual void Bind(int unit = 0) = 0;
    
    friend class GraphicsDevice;
};

}

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1
#include "pixelboost/graphics/device/gles1/texture.h"
#endif