#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/renderer/common/layer.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"

using namespace pb;

RenderLayer::RenderLayer(int depth, Camera* camera)
    : _Camera(camera)
    , _Depth(depth)
{
    
}

Camera* RenderLayer::GetCamera()
{
    return _Camera;
}

int RenderLayer::GetDepth()
{
    return _Depth;
}
