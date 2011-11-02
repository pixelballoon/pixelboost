#pragma once

namespace libpixel
{

class RenderLayer;
    
class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    virtual void Update(float time) = 0;
    virtual void Render(RenderLayer* layer) = 0;
};

}
