#pragma once

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"
#include "pixelboost/resource/resourceManager.h"

namespace pb
{
    
    class SpriteSheet;
    class TextureResource;
    
    class SpriteSheetResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        SpriteSheetResource(ResourcePool* pool, const std::string& filename);
        ~SpriteSheetResource();
        
        virtual ResourceReadyState IsReadyToProcess(ResourceProcess process, std::string& errorDetails);
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceProcess process);
        
    public:
        SpriteSheet* GetSpriteSheet();
        
    private:
        ResourceError LoadSheet(ResourcePool* pool, const std::string& filename, std::string& errorDetails);
        
        SpriteSheet* _SpriteSheet;
        std::shared_ptr<TextureResource> _Texture;
    };
    
};
