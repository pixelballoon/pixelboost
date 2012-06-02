#pragma once

#include <vector>

namespace pb
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
