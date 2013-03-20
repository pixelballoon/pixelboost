#include <algorithm>
#include <set>

#include "pixelboost/debug/assert.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/resource/resourceManager.h"

using namespace pb;

Renderer* Renderer::_Instance = 0;

Renderer::Renderer()
{
    PbAssert(!_Instance);
    
    _Instance = this;
    
    _TechniqueHandler = 0;
    
    ResourcePool* defaultPool = ResourceManager::Instance()->GetPool("pixelboost");
    defaultPool->CacheResource<ShaderResource>("/shaders/pb_solid.shc");
    defaultPool->CacheResource<ShaderResource>("/shaders/pb_solidColor.shc");
    defaultPool->CacheResource<ShaderResource>("/shaders/pb_textured.shc");
    defaultPool->CacheResource<ShaderResource>("/shaders/pb_texturedColor.shc");
}

Renderer::~Renderer()
{
    _Instance = 0;
}

Renderer* Renderer::Instance()
{
    return _Instance;
}

void Renderer::Render()
{
    GraphicsDevice::Instance()->ClearBuffers();
    
    for (const auto& viewport : _Viewports)
    {
        RenderViewport(viewport, kRenderPassScene);
    }
    
    for (const auto& viewport : _Viewports)
    {
        RenderViewport(viewport, kRenderPassUi);
    }
}

void Renderer::RenderViewport(Viewport* viewport, RenderPass pass, Uid schemeOverride)
{
    viewport->Render(pass);
    
    glm::mat4x4 projectionMatrix;
    glm::mat4x4 viewMatrix;
    
    switch (pass)
    {
        case kRenderPassScene:
            projectionMatrix = viewport->GetSceneCamera()->ProjectionMatrix;
            viewMatrix = viewport->GetSceneCamera()->ViewMatrix;
            break;
            
        case kRenderPassUi:
            projectionMatrix = viewport->GetUiCamera()->ProjectionMatrix;
            viewMatrix = viewport->GetUiCamera()->ViewMatrix;
            break;
    }
    
    FlushBuffer(viewport->GetNativeRegion(), schemeOverride ? schemeOverride : viewport->GetRenderScheme(), projectionMatrix, viewMatrix);
}

void Renderer::SetTechniqueHandler(TechniqueHandler* techniqueHandler)
{
    _TechniqueHandler = techniqueHandler;
}

ShaderTechnique* Renderer::GetTechnique(Uid techniqueId)
{
    if (!_TechniqueHandler)
        return 0;
    
    return _TechniqueHandler->GetTechnique(techniqueId);
}

void Renderer::AttachRenderable(Renderable* renderable)
{
    _Renderables[renderable->GetLayer()].push_back(renderable);
}

void Renderer::AddViewport(Viewport* viewport)
{
    _Viewports.push_back(viewport);
}

void Renderer::RemoveViewport(Viewport* viewport)
{
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        if (*it == viewport)
        {
            _Viewports.erase(it);
            return;
        }
    }
}

const Renderer::ViewportList& Renderer::GetViewports()
{
    return _Viewports;
}

void Renderer::SetHandler(int renderableType, IRenderer* renderer)
{
    _RenderableHandlers[renderableType] = renderer;
}

static bool RenderableBackToFrontSorter(const Renderable* a, const Renderable* b)
{
    return a->GetModelViewMatrix()[3][2] < b->GetModelViewMatrix()[3][2];
}

void Renderer::FlushBuffer(const glm::vec4& viewport, Uid renderScheme, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    for (int i=0; i<16; i++)
    {
        RenderableList& renderables = _Renderables[i];
        
        if (!renderables.size())
            continue;
        
        for (RenderableList::iterator it = renderables.begin(); it != renderables.end(); ++it)
        {
            (*it)->CalculateModelViewMatrix(viewMatrix);
        }
        
        std::stable_sort(renderables.begin(), renderables.end(), &RenderableBackToFrontSorter);
        
        Uid type = renderables[0]->GetType();
        Shader* shader = renderables[0]->GetShader();
        int start = 0;
        int count = 0;
        
        for (int i=0; i < renderables.size(); i++)
        {
            Uid newType = renderables[i]->GetType();
            Shader* newShader = renderables[i]->GetShader();
            
            if (type == newType && shader == newShader)
            {
                count++;
            } else {
                RenderBatch(count, &renderables[start], renderScheme, viewport, projectionMatrix, viewMatrix);
                start = i;
                count = 1;
                type = newType;
            }
        }
        
        if (count > 0)
        {
            RenderBatch(count, &renderables[start], renderScheme, viewport, projectionMatrix, viewMatrix);
        }
    }
    
    _Renderables.clear();
}

void Renderer::RenderBatch(int count, Renderable** renderable, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    GraphicsDevice::Instance()->ResetState();
    
    RenderableHandlerMap::iterator it = _RenderableHandlers.find(renderable[0]->GetType());
    
    if (it != _RenderableHandlers.end())
    {
        it->second->Render(count, renderable, renderScheme, viewport, projectionMatrix, viewMatrix);
    }
}
