#pragma once

#include <vector>

namespace libpixel
{

class Renderer;

class RenderLayer
{
public:
    RenderLayer();
    
    void Render();
    
    void AddRenderer(Renderer* renderer);
    void RemoveRenderer(Renderer* renderer);
    
private:
    typedef std::vector<Renderer*> RendererList;
    
    RendererList _Renderers;
};

}
