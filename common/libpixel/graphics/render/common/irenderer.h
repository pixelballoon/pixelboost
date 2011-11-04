#pragma once

namespace libpixel
{

class RenderLayer;
    
class IRenderer
{
public:
    IRenderer();
    virtual ~IRenderer();
    
    virtual void Update(float time) = 0;
    virtual void Render(RenderLayer* layer) = 0;
};

}
