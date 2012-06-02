#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/renderer/common/layer.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"

using namespace pb;

RenderLayer::RenderLayer(int depth)
    : _Depth(depth)
{
    
}

int RenderLayer::GetDepth()
{
    return _Depth;
}
