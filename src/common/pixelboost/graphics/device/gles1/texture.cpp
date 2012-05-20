#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/gles1/texture.h"

#include "lodepng/lodepng.h"

pb::TextureGLES1::TextureGLES1(GraphicsDeviceGLES1* device)
    : _Device(device)
{
    
}

pb::TextureGLES1::~TextureGLES1()
{
    glDeleteTextures(1, &_Texture);
}

void pb::TextureGLES1::Bind(int unit)
{
    _Device->BindTexture(this);
}

void pb::TextureGLES1::LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format)
{
    if (format != kTextureFormatRGBA)
    {
        printf("WARN: Only RGBA texture format is currently supported!");
        return;
    }
    
    glGenTextures(1, &_Texture);
    
    Texture* previousTexture = _Device->BindTexture(this);
    
    if (createMips)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	
    _Device->BindTexture(previousTexture);

}

void pb::TextureGLES1::LoadFromPng(const std::string& path, bool createMips)
{
    LodePNG::Decoder decoder;
    std::vector<unsigned char> file;
    std::vector<unsigned char> data;
    LodePNG::loadFile(file, path);
    decoder.decode(data, &file[0], file.size());
	
	LoadFromBytes(&data[0], decoder.getWidth(), decoder.getHeight(), createMips, kTextureFormatRGBA);
}

#endif
