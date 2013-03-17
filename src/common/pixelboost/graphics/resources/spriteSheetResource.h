#pragma once

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"
#include "pixelboost/resource/resource.h"

namespace pb
{
    
    class SpriteSheet;
    
    class SpriteSheetResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        SpriteSheetResource();
        ~SpriteSheetResource();
        
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceProcess process);
        
    public:
        SpriteSheet* GetSpriteSheet();
        
    private:
        SpriteSheet* _SpriteSheet;
    };
    
};
