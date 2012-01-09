#pragma once

#include <map>
#include <vector>

namespace pixelboost
{

class IRenderer;
class RenderLayer;
    
struct RenderItem
{
    int Renderer;
    void* Data;
};

class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    void Update(float time);
    void Render();
    
    void AddLayer(RenderLayer* layer);
    void RemoveLayer(RenderLayer* layer);
    
private:
    int AddRenderer(IRenderer* renderer);
    void RemoveRenderer(IRenderer* renderer);
    
    static bool LayerSortPredicate(const RenderLayer* a, const RenderLayer* b);
    
    typedef std::map<int, IRenderer*> RendererMap;
    typedef std::vector<RenderLayer*> LayerList;
    
    int _FreeRendererId;
    RendererMap _Renderers;
    LayerList _Layers;
    
    friend class IRenderer;
};

}
