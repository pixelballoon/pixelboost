#include <string.h>

#include "glm/gtx/bit.hpp"
#include "stbimage/stb_image.h"

#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/device/texture.h"

using namespace pb;

Texture::Texture()
#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    : _Data(0)
#endif
{
    
}

Texture::~Texture()
{
#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    if (_Data)
        delete[] _Data;
#endif
}

bool Texture::LoadFromBytes(const unsigned char* data, int width, int height, bool createMips, TextureFormat format)
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
                break;
            case kTextureFormatRGBA:
                _Data = new unsigned char[width*height*4];
                memcpy(_Data, data, width*height*4);
                break;
        }
    }
#endif

    return true;
}

bool Texture::LoadFromFile(const std::string& path, bool createMips)
{
    bool status = true;

    PbLogDebug("graphics.texture", "Loading texture from file (%s)\n", path.c_str());
    
    if (path.length() >= 4 && path.substr(path.length()-4) == ".jpa")
    {
        pb::File* file = pb::FileSystem::Instance()->OpenFile(path);
        if (!file)
            return false;
        
        int rgbLength;
        int alphaLength;

        file->Read(rgbLength);
        file->Read(alphaLength);
        
        unsigned char* encodedRgbData = new unsigned char[rgbLength];
        unsigned char* encodedAlphaData = new unsigned char[alphaLength];
        
        file->Read(encodedRgbData, rgbLength);
        file->Read(encodedAlphaData, alphaLength);
        
        int width, height, components;
        unsigned char* decodedRgb = stbi_load_from_memory(encodedRgbData, rgbLength, &width, &height, &components, STBI_rgb);
        unsigned char* decodedAlpha = stbi_load_from_memory(encodedAlphaData, alphaLength, &width, &height, &components, STBI_grey);
        
        unsigned char* decoded = new unsigned char[width*height*4];
        
        unsigned char* decodedTemp = decoded;
        unsigned char* rgbTemp = decodedRgb;
        unsigned char* alphaTemp = decodedAlpha;
        
        for (int y=0; y<height; y++)
        {
            for (int x=0; x<width; x++)
            {
                decodedTemp[0] = rgbTemp[0];
                decodedTemp[1] = rgbTemp[1];
                decodedTemp[2] = rgbTemp[2];
                decodedTemp[3] = *alphaTemp;
                
                decodedTemp += 4;
                rgbTemp += 3;
                alphaTemp++;
            }
        }
        
        stbi_image_free(decodedRgb);
        stbi_image_free(decodedAlpha);
        
        status = LoadFromBytes(decoded, width, height, createMips, kTextureFormatRGBA);
        
        delete[] decoded;
    } else {
        std::vector<unsigned char> data;
        unsigned char* decoded;

        pb::File* file = pb::FileSystem::Instance()->OpenFile(path);
        if (!file)
            return false;
        
        file->ReadAll(data);

        int width, height, components;
        decoded = stbi_load_from_memory(&data[0], data.size(), &width, &height, &components, STBI_default);

        if (!glm::isPowerOfTwo(width) || !glm::isPowerOfTwo(height))
            createMips = false;
        
        delete file;
        
        status = LoadFromBytes(decoded, width, height, createMips, components == 3 ? kTextureFormatRGB : kTextureFormatRGBA);
        
        stbi_image_free(decoded);
    }

    if (status)
    {
        PbLogDebug("graphics.texture", "Loaded texture from file (%s)\n", path.c_str());
    }
    else
    {
        PbLogError("graphics.texture", "Failed to load texture from file (%s)\n", path.c_str());
    }
    
    return status;
}

const glm::vec2& Texture::GetSize() const
{
    return _Size;
}

#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
void Texture::OnContextLost()
{
    LoadFromBytes(_Data, _Size.x, _Size.y, _DataCreateMips, _DataFormat);
}
#endif