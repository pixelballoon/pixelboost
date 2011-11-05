#include "libpixel/graphics/camera/camera.h"
#include "libpixel/graphics/render/common/layer.h"
#include "libpixel/graphics/render/common/irenderer.h"

using namespace libpixel;

RenderLayer::RenderLayer(Camera* camera)
    : _Camera(camera)
{
    
}
