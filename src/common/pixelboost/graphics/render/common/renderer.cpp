#include <algorithm>
#include <set>

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/render/common/layer.h"
#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/graphics/render/common/renderer.h"

using namespace pixelboost;

Renderer* Renderer::Renderer::_Instance = 0;

Renderer::Renderer()
    : _FreeRendererId(0)
{
    _Instance = this;
    _DefaultCamera = new OrthographicCamera();
    _DefaultLayer = new RenderLayer(-1, _DefaultCamera);
    AddLayer(_DefaultLayer);
}

Renderer::~Renderer()
{
    _Instance = 0;
    RemoveLayer(_DefaultLayer);
}

Renderer* Renderer::Instance()
{
    return _Instance;
}

void Renderer::Update(float time)
{
    for (RendererMap::iterator it = _Renderers.begin(); it != _Renderers.end(); ++it)
    {
        it->second->Update(time);
    }
}

RenderLayer* Renderer::GetDefaultLayer()
{
    return _DefaultLayer;
}

void Renderer::Render()
{
    std::sort(_Layers.begin(), _Layers.end(), &Renderer::LayerSortPredicate);
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        (*it)->_Camera->ApplyTransform();
        for (RendererMap::iterator rendererIt = _Renderers.begin(); rendererIt != _Renderers.end(); ++rendererIt)
        {
            rendererIt->second->Render(*it == _DefaultLayer ? 0 : *it);
        }
    }
}

int Renderer::AddRenderer(IRenderer* renderer)
{
    int id = _FreeRendererId++;
    _Renderers[id] = renderer;
    return id;
}

void Renderer::RemoveRenderer(IRenderer* renderer)
{
    _Renderers.erase(_Renderers.find(renderer->GetId()));
}

void Renderer::AddLayer(RenderLayer* layer)
{
    _Layers.push_back(layer);
}

void Renderer::RemoveLayer(RenderLayer* layer)
{
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        if (*it == layer)
        {
            _Layers.erase(it);
            return;
        }
    }
}

bool Renderer::LayerSortPredicate(const RenderLayer* a, const RenderLayer* b)
{
    return a->_Depth < b->_Depth;
}
