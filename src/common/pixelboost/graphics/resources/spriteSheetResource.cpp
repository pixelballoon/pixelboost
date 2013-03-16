#include "pixelboost/graphics/resources/spriteSheetResource.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::SpriteSheetResource)

SpriteSheetResource::SpriteSheetResource()
{
    
}

SpriteSheetResource::~SpriteSheetResource()
{
    
}

bool SpriteSheetResource::ProcessResource(ResourceState state, const std::string& filename, std::string& error)
{
    switch (state)
    {
        default:
            return false;
            break;
    }
}

ResourceThread SpriteSheetResource::GetResourceThread(ResourceState state)
{
    return kResourceThreadMain;
}