#include "glm/gtx/bit.hpp"
#include "stbimage/stb_image.h"

#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/resources/textureResource.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::TextureResource)

TextureResource::TextureResource(ResourcePool* pool, const std::string& filename)
    : Resource(pool, filename)
{
    _Texture = 0;
    _DecodedCustom = 0;
    _DecodedSTB = 0;
    _Width = 0;
    _Height = 0;
    _Components = 0;
    _CreateMips = true;
}

TextureResource::~TextureResource()
{
    
}

ResourceError TextureResource::ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        {
            if (!LoadFile(filename))
            {
                return kResourceErrorNoSuchResource;
            }
            return kResourceErrorNone;
        }
            
        case kResourceProcessProcess:
        {
            if (!Decode(filename))
            {
                errorDetails = "Error decoding texture file";
                return kResourceErrorSystemError;
            }
            return kResourceErrorNone;
        }
            
        case kResourceProcessPostProcess:
        {
            if (!Upload())
            {
                errorDetails = "Error uploading texture to graphics card";
                return kResourceErrorSystemError;
            }
            return kResourceErrorNone;
        }
            
        case kResourceProcessUnload:
        {
            if (_Texture)
            {
                pb::GraphicsDevice::Instance()->DestroyTexture(_Texture);
                _Texture = 0;
            }
            if (_DecodedCustom)
            {
                delete[] _DecodedCustom;
                _DecodedCustom = 0;
            }
            if (_DecodedSTB)
            {
                stbi_image_free(_DecodedSTB);
                _DecodedSTB = 0;
            }
            std::vector<unsigned char>().swap(_FileData);
            return kResourceErrorNone;
        }
    }
}

ResourceThread TextureResource::GetResourceThread(ResourceProcess process)
{
    if (process == kResourceProcessPostProcess || process == kResourceProcessUnload)
        return kResourceThreadMain;
        
    return kResourceThreadAny;
}

Texture* TextureResource::GetTexture()
{
    return _Texture;
}

bool TextureResource::LoadFile(const std::string& filename)
{
    auto file = pb::FileSystem::Instance()->OpenFile(filename);
    if (!file)
        return false;
    
    file->ReadAll(_FileData);
    
    return true;
}

bool TextureResource::Decode(const std::string& filename)
{
    bool status = false;
    
    if (filename.length() >= 4 && filename.substr(filename.length()-4) == ".jpa")
    {
        int rgbLength;
        int alphaLength;
        
        memcpy(&rgbLength, &_FileData[0], 4);
        memcpy(&alphaLength, &_FileData[4], 4);
        
        unsigned char* decodedRgb = stbi_load_from_memory(&_FileData[8], rgbLength, &_Width, &_Height, &_Components, STBI_rgb);
        unsigned char* decodedAlpha = stbi_load_from_memory(&_FileData[8+rgbLength], alphaLength, &_Width, &_Height, &_Components, STBI_grey);
        
        if (!_Width || !_Height)
        {
            delete decodedRgb;
            delete decodedAlpha;
            return false;
        }
        
        std::vector<unsigned char>().swap(_FileData);
        
        _DecodedCustom = new unsigned char[_Width*_Height*4];
        
        unsigned char* decodedTemp = _DecodedCustom;
        unsigned char* rgbTemp = decodedRgb;
        unsigned char* alphaTemp = decodedAlpha;
        
#ifdef PIXELBOOST_PLATFORM_ANDROID
        for (int y=0; y<_Height; y++)
        {
            for (int x=0; x<_Width; x++)
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
#else
        for (int y=0; y<_Height; y++)
        {
            for (int x=0; x<_Width; x++)
            {
                decodedTemp[2] = rgbTemp[0];
                decodedTemp[1] = rgbTemp[1];
                decodedTemp[0] = rgbTemp[2];
                decodedTemp[3] = *alphaTemp;
                
                decodedTemp += 4;
                rgbTemp += 3;
                alphaTemp++;
            }
        }
#endif
        
        stbi_image_free(decodedRgb);
        stbi_image_free(decodedAlpha);
        
        status = true;
    } else {
        _DecodedSTB = stbi_load_from_memory(&_FileData[0], _FileData.size(), &_Width, &_Height, &_Components, STBI_default);
        
        std::vector<unsigned char>().swap(_FileData);
        
        if (_Components == 4)
        {
            for (int i=0; i<_Width*_Height*4; i+=4)
            {
                float alpha = static_cast<float>(_DecodedSTB[i+3])/255.f;
                _DecodedSTB[i+0] *= alpha;
                _DecodedSTB[i+1] *= alpha;
                _DecodedSTB[i+2] *= alpha;
            }
        }
        
        status = true;
    }
    
    return status;
}

bool TextureResource::Upload()
{
    bool status = false;
    
    if (!glm::isPowerOfTwo(_Width) || !glm::isPowerOfTwo(_Height))
        _CreateMips = false;

    _Texture = pb::GraphicsDevice::Instance()->CreateTexture();
    
    if (_DecodedCustom)
    {
#ifdef PIXELBOOST_PLATFORM_ANDROID
        Texture::TextureFormat format = Texture::kTextureFormatRGBA;
#else
        Texture::TextureFormat format = Texture::kTextureFormatBGRA;
#endif
        
        status = _Texture->LoadFromBytes(_DecodedCustom, _Width, _Height, _CreateMips, format);
        
        delete[] _DecodedCustom;
        _DecodedCustom = 0;
    } else if (_DecodedSTB)
    {
        status = _Texture->LoadFromBytes(_DecodedSTB, _Width, _Height, _CreateMips, _Components == 3 ? Texture::kTextureFormatRGB : Texture::kTextureFormatRGBA);
        
        stbi_image_free(_DecodedSTB);
        _DecodedSTB = 0;
    }
    
    return status;
}
