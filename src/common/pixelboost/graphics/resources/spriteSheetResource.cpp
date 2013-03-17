#include "pixelboost/graphics/resources/spriteSheetResource.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::SpriteSheetResource)

SpriteSheetResource::SpriteSheetResource()
{
    
}

SpriteSheetResource::~SpriteSheetResource()
{
    
}

bool SpriteSheetResource::ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails)
{
    switch (state)
    {
        default:
            error = kResourceErrorSystemError;
            errorDetails = "Loader not yet implemented";
            return true;
    }
}

ResourceThread SpriteSheetResource::GetResourceThread(ResourceState state)
{
    return kResourceThreadMain;
}