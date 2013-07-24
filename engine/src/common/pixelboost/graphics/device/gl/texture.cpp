#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL

#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/device/gl/texture.h"

using namespace pb;

TextureGL::TextureGL(GraphicsDeviceGL* device)
    : _Device(device)
    , _Texture(0)
{
    glGenTextures(1, &_Texture);
}

TextureGL::~TextureGL()
{
    glDeleteTextures(1, &_Texture);
}

bool TextureGL::LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format)
{
    if (!Texture::LoadFromBytes(data, width, height, createMips, format))
        return false;

    if (format != kTextureFormatRGBA && format != kTextureFormatBGRA && format != kTextureFormatRGB)
    {
        PbLogWarn("graphics.texture", "Only RGBA, BGRA and RGB texture formats are currently supported!");
        return false;
    }
    
    Texture* previousTexture = _Device->BindTexture(0, this, true);
    
#ifdef PIXELBOOST_PLATFORM_ANDROID
    createMips = false;
#endif
    
    if (createMips)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    switch (format)
    {
        case Texture::kTextureFormatRGB:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case Texture::kTextureFormatRGBA:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        case Texture::kTextureFormatBGRA:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
            break;
    }

    if (createMips)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
	
    _Device->BindTexture(0, previousTexture);

    _Size = glm::vec2(width, height);
        
    return true;
}

void TextureGL::Bind(int textureUnit)
{
    _Device->BindTexture(textureUnit, this);
}

#endif
