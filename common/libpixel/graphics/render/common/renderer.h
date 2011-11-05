#pragma once

#include <vector>

namespace libpixel
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
    
    void AddLayer(int depth, RenderLayer* layer);
    void RemoveLayer(RenderLayer* layer);
    
    void SetLayerDepth(const RenderLayer* layer, int depth);
    
private:
    struct InternalRenderLayer
    {
        InternalRenderLayer(int depth, RenderLayer* layer);
        
        int depth;
        RenderLayer* layer;
    };

    static bool LayerSortPredicate(const InternalRenderLayer& a, const InternalRenderLayer& b);
    
    typedef std::vector<IRenderer*> RendererList;
    typedef std::vector<InternalRenderLayer> LayerList;
    
    RendererList _Renderers;
    LayerList _Layers;
};

}
