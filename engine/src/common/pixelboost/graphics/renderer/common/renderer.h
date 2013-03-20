#pragma once

#include <map>
#include <set>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/framework/definitions.h"
#include "pixelboost/graphics/definitions.h"

namespace pb
{

    class Camera;
    class IRenderer;
    class OrthographicCamera;
    class Renderable;
    class Shader;
    class ShaderTechnique;
    class Viewport;
        
    struct RenderItem
    {
        int Renderer;
        void* Data;
    };
        
    class TechniqueHandler
    {
    public:
        virtual ShaderTechnique* GetTechnique(Uid techniqueId) = 0;
    };

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();
        
        static Renderer* Instance();
            
        void Render();
        void RenderViewport(Viewport* viewport, RenderPass pass, Uid schemeOverride = 0);
        
        void SetTechniqueHandler(TechniqueHandler* techniqueHandler);
        ShaderTechnique* GetTechnique(Uid techniqueId);
        
        void AddViewport(Viewport* viewport);
        void RemoveViewport(Viewport* viewport);
        
    public:
        typedef std::vector<Viewport*> ViewportList;
        const ViewportList& GetViewports();
        
        void SetHandler(int renderableType, IRenderer* renderer);
        
    private:
        void AttachRenderable(Renderable* renderable);
        
        void FlushBuffer(const glm::vec4& viewport, Uid renderScheme, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
        void RenderBatch(int count, Renderable** renderable, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
        
    private:
        typedef std::map<int, IRenderer*> RenderableHandlerMap;
        typedef std::vector<Renderable*> RenderableList;
        typedef std::map<int, RenderableList> LayerRenderableMap;
        
    private:
        TechniqueHandler* _TechniqueHandler;
        
        RenderableHandlerMap _RenderableHandlers;
        ViewportList _Viewports;
        
        LayerRenderableMap _Renderables;
        
        static Renderer* _Instance;
        
        friend class IRenderer;
        friend class RenderSystem;
        friend class Viewport;
    };

}
