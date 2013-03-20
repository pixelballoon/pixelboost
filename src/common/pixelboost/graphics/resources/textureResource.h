#pragma once

#include <string>

#include "pixelboost/resource/resource.h"

namespace pb
{
    
    class Texture;
    
    class TextureResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        TextureResource(ResourcePool* pool, const std::string& filename);
        ~TextureResource();
        
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceProcess process);
        
    public:
        Texture* GetTexture();
    
        bool LoadFile(const std::string& filename);
        bool Decode(const std::string& filename);
        bool Upload();
    
    private:
        Texture* _Texture;
    
        std::vector<unsigned char> _FileData;
        unsigned char* _DecodedCustom;
        unsigned char* _DecodedSTB;
 
        bool _CreateMips;
        int _Width;
        int _Height;
        int _Components;
    };
    
}