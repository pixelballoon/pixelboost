#include "pixelboost/graphics/resources/spriteSheetResource.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::SpriteSheetResource)

SpriteSheetResource::SpriteSheetResource()
{
    
}

SpriteSheetResource::~SpriteSheetResource()
{
    
}

ResourceError SpriteSheetResource::ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails)
{
    switch (process)
    {
        default:
            errorDetails = "Loader not yet implemented";
            return kResourceErrorSystemError;
    }
}

ResourceThread SpriteSheetResource::GetResourceThread(ResourceProcess process)
{
    return kResourceThreadMain;
}