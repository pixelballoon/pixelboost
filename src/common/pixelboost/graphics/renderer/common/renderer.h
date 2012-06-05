#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <map>
#include <set>
#include <vector>

namespace pb
{

class Effect;
class EffectManager;
class IRenderer;
class OrthographicCamera;
class Renderable;
class Viewport;
    
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
    
    static Renderer* Instance();
        
    void Render();
    
    EffectManager* GetEffectManager();
    
    // This should only be called during a Render callback
    void AddItem(Renderable* renderable);
    
    void AddViewport(Viewport* viewport);
    void RemoveViewport(Viewport* viewport);
    
    void SetHandler(int renderableType, IRenderer* renderer);
    
private:
    void FlushBuffer(Viewport* viewport);
    void RenderBatch(Viewport* viewport, int count, Renderable** renderable, Effect* effect);
    
    void AddRenderer(IRenderer* renderer);
    void RemoveRenderer(IRenderer* renderer);
    
private:
    typedef std::map<int, IRenderer*> RenderableHandlerMap;
    typedef std::set<IRenderer*> RendererSet;
    typedef std::vector<Viewport*> ViewportList;
    typedef std::vector<Renderable*> RenderableList;
    typedef std::map<int, RenderableList> LayerRenderableMap;
    
private:
    EffectManager* _EffectManager;
    
    RenderableHandlerMap _RenderableHandlers;
    RendererSet _Renderers;
    ViewportList _Viewports;
    
    LayerRenderableMap _Renderables;
    
    static Renderer* _Instance;
    
    friend class IRenderer;
    friend class Viewport;
};

}

#endif
