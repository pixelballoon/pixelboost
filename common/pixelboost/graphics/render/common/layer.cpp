#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/render/common/layer.h"
#include "pixelboost/graphics/render/common/irenderer.h"

using namespace pixelboost;

RenderLayer::RenderLayer(int depth, Camera* camera)
    : _Camera(camera)
    , _Depth(depth)
{
    
}

int RenderLayer::GetDepth()
{
    return _Depth;
}
