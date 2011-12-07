#pragma once

#include <vector>

namespace pixelboost
{

class IRenderer;
class RenderLayer;

class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    void Update(float time);
    void Render();
    
    void AddRenderer(IRenderer* renderer);
    void RemoveRenderer(IRenderer* renderer);
    
    void AddLayer(RenderLayer* layer);
    void RemoveLayer(RenderLayer* layer);
    
private:
    static bool LayerSortPredicate(const RenderLayer* a, const RenderLayer* b);
    
    typedef std::vector<IRenderer*> RendererList;
    typedef std::vector<RenderLayer*> LayerList;
    
    RendererList _Renderers;
    LayerList _Layers;
};

}
