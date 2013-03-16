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
        TextureResource();
        ~TextureResource();
        
        virtual bool ProcessResource(ResourceState state, const std::string& filename, std::string& error);
        virtual ResourceThread GetResourceThread(ResourceState state);
        
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