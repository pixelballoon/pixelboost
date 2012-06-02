#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/logic/game.h"
#include "pixelboost/graphics/renderer/common/layer.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"

using namespace pb;

Viewport::Viewport(int viewportId)
    : _ViewportId(viewportId)
{
    
}

Viewport::~Viewport()
{
    
}

void Viewport::AddLayer(RenderLayer* layer, Camera* camera)
{
    _Layers.push_back(layer);
    _Cameras[layer] = camera;
}

void Viewport::RemoveLayer(RenderLayer* layer)
{
    CameraMap::iterator it = _Cameras.find(layer);
    if (it != _Cameras.end())
        _Cameras.erase(it);
    
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        if (*it == layer)
        {
            _Layers.erase(it);
            return;
        }
    }
}

void Viewport::Render()
{
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        _Cameras[*it]->CalculateTransform(0);
        GraphicsDevice::Instance()->SetMatrix(GraphicsDevice::kMatrixTypeModelView, _Cameras[*it]->ModelView);
        GraphicsDevice::Instance()->SetMatrix(GraphicsDevice::kMatrixTypeProjection, _Cameras[*it]->Projection);
        
        for (Renderer::RendererMap::iterator rendererIt = pb::Game::Instance()->GetRenderer()->_Renderers.begin(); rendererIt != pb::Game::Instance()->GetRenderer()->_Renderers.end(); ++rendererIt)
        {
            rendererIt->second->Render(*it);
        }
    }
}
