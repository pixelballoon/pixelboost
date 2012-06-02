#include "glm/gtx/bit.hpp"
#include "lodepng/lodepng.h"

#include "pixelboost/graphics/device/texture.h"

using namespace pb;

Texture::Texture()
{
    
}

Texture::~Texture()
{
    
}

void Texture::LoadFromPng(const std::string& path, bool createMips)
{
    LodePNG::Decoder decoder;
    std::vector<unsigned char> file;
    std::vector<unsigned char> data;
    LodePNG::loadFile(file, path);
    decoder.decode(data, &file[0], file.size());
	
    if (!glm::isPowerOfTwo(decoder.getHeight()) || !glm::isPowerOfTwo(decoder.getWidth()))
        createMips = false;
    
	LoadFromBytes(&data[0], decoder.getWidth(), decoder.getHeight(), createMips, kTextureFormatRGBA);
}

const glm::vec2& Texture::GetSize()
{
    return _Size;
}
