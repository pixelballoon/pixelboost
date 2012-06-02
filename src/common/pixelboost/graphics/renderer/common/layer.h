#pragma once

#include <vector>

namespace pb
{

class Camera;
class IRenderer;

class RenderLayer
{
public:
    RenderLayer(int depth);
    
    int GetDepth();
    
private:
    int _Depth;
    
    friend class Renderer;
};

}
