#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2

#include "pixelboost/graphics/device/gles2/texture.h"

using namespace pb;

TextureGLES2::TextureGLES2(GraphicsDeviceGLES2* device)
    : _Device(device)
{
    
}

TextureGLES2::~TextureGLES2()
{
    glDeleteTextures(1, &_Texture);
}

void TextureGLES2::LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format)
{
    if (format != kTextureFormatRGBA)
    {
        printf("WARN: Only RGBA texture format is currently supported!");
        return;
    }
    
    glGenTextures(1, &_Texture);
    
    Texture* previousTexture = _Device->BindTexture(this);
    
    // TODO : Add create mips back in
    /*if (createMips)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }
    else*/
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	
    _Device->BindTexture(previousTexture);

    _Size = glm::vec2(width, height);
}

void TextureGLES2::Bind(int unit)
{
    _Device->BindTexture(this);
}

#endif
