#pragma once

#include <vector>

namespace libpixel
{

class Camera;
class IRenderer;

class RenderLayer
{
public:
    RenderLayer(Camera* camera);
    
private:
    Camera* _Camera;
    
    friend class Renderer;
};

}
