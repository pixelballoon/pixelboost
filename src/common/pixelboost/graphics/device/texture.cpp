#include "glm/gtx/bit.hpp"
#include "lodepng/lodepng.h"

#include "pixelboost/file/fileSystem.h"
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
    std::vector<unsigned char> data;
    std::vector<unsigned char> decoded;

    pb::File* file = pb::FileSystem::Instance()->OpenFile(pb::kFileLocationBundle, path);
    file->ReadAll(data);
    decoder.decode(decoded, &data[0], data.size());
	
    if (!glm::isPowerOfTwo(decoder.getHeight()) || !glm::isPowerOfTwo(decoder.getWidth()))
        createMips = false;
    
	LoadFromBytes(&decoded[0], decoder.getWidth(), decoder.getHeight(), createMips, kTextureFormatRGBA);
}

const glm::vec2& Texture::GetSize()
{
    return _Size;
}
