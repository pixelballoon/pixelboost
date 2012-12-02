#include <string.h>

#include "glm/gtx/bit.hpp"
#include "stbimage/stb_image.h"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/device/texture.h"

using namespace pb;

Texture::Texture()
    : _Data(0)
{
    
}

Texture::~Texture()
{
    if (_Data)
        delete[] _Data;
}

bool Texture::LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format, bool hasPremultipliedAlpha)
{
#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    if (_Data != 0 && _Data != data)
    {
        delete[] _Data;
        _Data = 0;
    }
    
    _DataFormat = format;
    _DataCreateMips = createMips;
    _Size = glm::vec2(width, height);
    
    if (_Data == 0)
    {
        switch (format)
        {
            case kTextureFormatRGB:
                _Data = new unsigned char[width*height*3];
                memcpy(_Data, data, width*height*3);
            case kTextureFormatRGBA:
                _Data = new unsigned char[width*height*4];
                memcpy(_Data, data, width*height*4);
                break;
        }
    }
#endif
    
    _HasPremultipliedAlpha = hasPremultipliedAlpha;
    
    return true;
}

bool Texture::LoadFromFile(pb::FileLocation location, const std::string& path, bool createMips, bool hasPremultipliedAlpha)
{
    std::vector<unsigned char> data;
    unsigned char* decoded;

    pb::File* file = pb::FileSystem::Instance()->OpenFile(location, path);
    if (!file)
        return false;
    
    file->ReadAll(data);
    
    int width, height, components;
    decoded = stbi_load_from_memory(&data[0], data.size(), &width, &height, &components, STBI_default);
	
    if (!glm::isPowerOfTwo(width) || !glm::isPowerOfTwo(height))
        createMips = false;
    
    delete file;
    
    bool status = LoadFromBytes(decoded, width, height, createMips, components == 3 ? kTextureFormatRGB : kTextureFormatRGBA, hasPremultipliedAlpha);
    
    stbi_image_free(decoded);
    
    return status;
}

const glm::vec2& Texture::GetSize() const
{
    return _Size;
}

bool Texture::HasPremultipliedAlpha() const
{
    return _HasPremultipliedAlpha;
}

void Texture::OnContextLost()
{
    LoadFromBytes(_Data, _Size.x, _Size.y, _DataCreateMips, _DataFormat, _HasPremultipliedAlpha);
}
