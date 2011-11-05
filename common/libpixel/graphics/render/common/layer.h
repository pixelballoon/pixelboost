#pragma once

#include <vector>

namespace libpixel
{

class Camera;
class IRenderer;

class RenderLayer
{
public:
    RenderLayer(int depth, Camera* camera);
    
    int GetDepth();
    
private:
    Camera* _Camera;
    int _Depth;
    
    friend class Renderer;
};

}
