#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <algorithm>
#include <set>

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"

using namespace pb;

Renderer* Renderer::_Instance = 0;

Renderer::Renderer()
{
    _Instance = this;
    
    _ShaderManager = new ShaderManager();
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
    
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        (*it)->Render(kRenderPassScene);
        
        FlushBuffer(*it, (*it)->GetSceneCamera());
    }
    
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        (*it)->Render(kRenderPassUi);
        
        FlushBuffer(*it, (*it)->GetUiCamera());
    }
}

ShaderManager* Renderer::GetShaderManager()
{
    return _ShaderManager;
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
    return a->GetModelViewMatrix()[3][2] > b->GetModelViewMatrix()[3][2];
}

void Renderer::FlushBuffer(Viewport* viewport, Camera* camera)
{
    for (int i=0; i<16; i++)
    {
        RenderableList& renderables = _Renderables[i];
        
        if (!renderables.size())
            continue;
        
        for (RenderableList::iterator it = renderables.begin(); it != renderables.end(); ++it)
        {
            (*it)->CalculateModelViewMatrix(viewport, camera);
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
                RenderBatch(viewport, count, &renderables[start], shader, camera);
                start = i;
                count = 1;
                type = newType;
                shader = newShader;
            }
        }
        
        if (count > 0)
        {
            RenderBatch(viewport, count, &renderables[start], shader, camera);
        }
    }
    
    _Renderables.clear();
}

void Renderer::RenderBatch(Viewport* viewport, int count, Renderable** renderable, Shader* shader, Camera* camera)
{
    if (!shader)
        return;
    
    RenderableHandlerMap::iterator it = _RenderableHandlers.find(renderable[0]->GetType());
    
    if (it != _RenderableHandlers.end())
    {
        ShaderTechnique* technique = shader->GetTechnique(viewport->GetRenderScheme());
        
        if (!technique)
        {
            for (int i=0; i < count; i++)
            {
                technique = viewport->GetTechnique(renderable[i], shader);
                
                if (technique)
                {
                    for (int j=0; j<technique->GetNumPasses(); j++)
                    {
                        ShaderPass* pass = technique->GetPass(j);
                        pass->Bind();
                        pass->GetShaderProgram()->SetUniform("PB_ProjectionMatrix", camera->ProjectionMatrix);
                        it->second->Render(1, &renderable[i], viewport, pass);
                    }
                }
            }
        } else
        {
            for (int i=0; i<technique->GetNumPasses(); i++)
            {
                ShaderPass* pass = technique->GetPass(i);
                pass->Bind();
                pass->GetShaderProgram()->SetUniform("PB_ProjectionMatrix", camera->ProjectionMatrix);
                it->second->Render(count, renderable, viewport, pass);
            }
        }
    }
}

#endif
