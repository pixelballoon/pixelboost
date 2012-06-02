#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <map>
#include <vector>

namespace pb
{

class IRenderer;
class OrthographicCamera;
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
    
    void Update(float time);
    void Render();
    
    void AddViewport(Viewport* viewport);
    void RemoveViewport(Viewport* viewport);
    
private:
    int AddRenderer(IRenderer* renderer);
    void RemoveRenderer(IRenderer* renderer);
    
    typedef std::map<int, IRenderer*> RendererMap;
    typedef std::vector<Viewport*> ViewportList;
    
    int _FreeRendererId;
    RendererMap _Renderers;
    ViewportList _Viewports;
    
    static Renderer* _Instance;
    
    friend class IRenderer;
    friend class Viewport;
};

}

#endif
