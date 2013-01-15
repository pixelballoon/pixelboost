#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <map>
#include <set>
#include <vector>

namespace pb
{

class Camera;
class IRenderer;
class OrthographicCamera;
class Renderable;
class Shader;
class ShaderManager;
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
    
    ShaderManager* GetShaderManager();
    
    void AddViewport(Viewport* viewport);
    void RemoveViewport(Viewport* viewport);
    
public:
    typedef std::vector<Viewport*> ViewportList;
    const ViewportList& GetViewports();
    
    void SetHandler(int renderableType, IRenderer* renderer);
    
private:
    void AttachRenderable(Renderable* renderable);
    
    void FlushBuffer(Viewport* viewport, Camera* camera);
    void RenderBatch(Viewport* viewport, int count, Renderable** renderable, Shader* shader, Camera* camera);
    
private:
    typedef std::map<int, IRenderer*> RenderableHandlerMap;
    typedef std::vector<Renderable*> RenderableList;
    typedef std::map<int, RenderableList> LayerRenderableMap;
    
private:
    ShaderManager* _ShaderManager;
    
    RenderableHandlerMap _RenderableHandlers;
    ViewportList _Viewports;
    
    LayerRenderableMap _Renderables;
    
    static Renderer* _Instance;
    
    friend class IRenderer;
    friend class RenderSystem;
    friend class Viewport;
};

}

#endif
