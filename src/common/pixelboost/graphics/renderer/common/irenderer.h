#pragma once

namespace pb
{

class EffectPass;
class RenderLayer;
class Renderable;
class Viewport;
    
class IRenderer
{
public:
    IRenderer();
    virtual ~IRenderer();
    
    virtual void Render(int count, Renderable* renderables, Viewport* viewport, EffectPass* effect) = 0;
};

}
