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
        
        virtual bool ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceState state);
        
    public:
        SpriteSheet* GetSpriteSheet();
        
    private:
        SpriteSheet* _SpriteSheet;
    };
    
};
