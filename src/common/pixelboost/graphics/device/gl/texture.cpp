#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL

#include "pixelboost/graphics/device/gl/texture.h"

using namespace pb;

TextureGL::TextureGL(GraphicsDeviceGL* device)
    : _Device(device)
{
    
}

TextureGL::~TextureGL()
{
    glDeleteTextures(1, &_Texture);
}

void TextureGL::LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format)
{
    Texture::LoadFromBytes(data, width, height, createMips, format);
    
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

void TextureGL::Bind(int unit)
{
    _Device->BindTexture(this);
}

#endif
