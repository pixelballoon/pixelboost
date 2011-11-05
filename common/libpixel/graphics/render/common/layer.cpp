#include "libpixel/graphics/camera/camera.h"
#include "libpixel/graphics/render/common/layer.h"
#include "libpixel/graphics/render/common/irenderer.h"

using namespace libpixel;

RenderLayer::RenderLayer(int depth, Camera* camera)
    : _Camera(camera)
    , _Depth(depth)
{
    
}

int RenderLayer::GetDepth()
{
    return _Depth;
}
