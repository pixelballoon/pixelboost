#pragma once

namespace pb
{

class RenderLayer;
class Renderable;
class ShaderPass;
class Viewport;
    
class IRenderer
{
public:
    IRenderer();
    virtual ~IRenderer();
    
    virtual void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass) = 0;
};

}
