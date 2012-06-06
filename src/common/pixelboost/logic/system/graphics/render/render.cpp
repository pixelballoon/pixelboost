#include "pixelboost/logic/system/graphics/render/render.h"

using namespace pb;

Uid RenderSystem::GetType()
{
    return GetStaticType();
}

Uid RenderSystem::GetStaticType()
{
    return TypeHash("render");
}
