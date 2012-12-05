#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/file/fileHelpers.h"
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
        kTextureFormatRGB,
        kTextureFormatRGBA,
    };
    
    virtual bool LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format, bool hasPremultipliedAlpha);
    virtual bool LoadFromFile(FileLocation location, const std::string& image, bool createMips, bool hasPremultipliedAlpha);
    
    const glm::vec2& GetSize() const;
    bool HasPremultipliedAlpha() const;

#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    void OnContextLost();
#endif
    
protected:
    virtual void Bind(int unit = 0) = 0;
    
protected:
#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    TextureFormat _DataFormat;
    unsigned char* _Data;
    bool _DataCreateMips;
#endif
    bool _HasPremultipliedAlpha;
    
    glm::vec2 _Size;
    
    friend class GraphicsDevice;
};

}

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1
#include "pixelboost/graphics/device/gles1/texture.h"
#endif

#ifdef PIXELBOOST_GRAPHICS_OPENGL
#include "pixelboost/graphics/device/gl/texture.h"
#endif
