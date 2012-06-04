#pragma once

namespace pb
{

class RenderLayer;
class Renderable;
class Viewport;
    
class IRenderer
{
public:
    IRenderer();
    virtual ~IRenderer();
    
    virtual void Update(float time) = 0;
    virtual void Render(int count, Renderable* renderables, Viewport* viewport) = 0;
};

}
