#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/logic/system/graphics/render/render.h"

using namespace pb;

Renderable::Renderable(Uid entityUid)
    : _BoundsDirty(true)
    , _Layer(0)
    , _Effect(0)
    , _EntityUid(entityUid)
    , _RenderPass(kRenderPassScene)
    , _WorldMatrixDirty(true)
{
    
}

Renderable::Renderable(Uid entityUid, Effect* effect)
    : _BoundsDirty(true)
    , _Layer(0)
    , _Effect(effect)
    , _EntityUid(entityUid)
    , _RenderPass(kRenderPassScene)
    , _WorldMatrixDirty(true)
{
    
}

Renderable::~Renderable()
{
    
}

void Renderable::SetSystem(RenderSystem* system)
{
    _System = system;
}

Uid Renderable::GetEntityUid()
{
    return _EntityUid;
}

void Renderable::SetRenderPass(RenderPass renderPass)
{
    _RenderPass = renderPass;
    RefreshSystemBinding();
}

RenderPass Renderable::GetRenderPass()
{
    return _RenderPass;
}

void Renderable::SetLayer(int layer)
{
    _Layer = layer;
}

int Renderable::GetLayer()
{
    return _Layer;
}

void Renderable::DirtyBounds()
{
    _BoundsDirty = true;
}

void Renderable::SetBounds(const BoundingSphere& bounds)
{
    _Bounds = bounds;
    _BoundsDirty = false;
    
    if (_System)
        _System->UpdateBounds(this);
}

const BoundingSphere& Renderable::GetBounds()
{
    if (_BoundsDirty)
        CalculateBounds();
    
    return _Bounds;
}

void Renderable::DirtyWorldMatrix()
{
    _WorldMatrixDirty = true;
}

void Renderable::SetWorldMatrix(const glm::mat4x4& matrix)
{
    _WorldMatrix = matrix;
    _WorldMatrixDirty = false;
}

const glm::mat4x4& Renderable::GetWorldMatrix()
{
    if (_WorldMatrixDirty)
        CalculateWorldMatrix();
    
    return _WorldMatrix;
}

void Renderable::CalculateMVP(Viewport* viewport, Camera* camera)
{
    if (_WorldMatrixDirty)
        CalculateWorldMatrix();
    
    _MVPMatrix = camera->ViewProjectionMatrix * _WorldMatrix;
}

const glm::mat4x4& Renderable::GetMVP() const
{
    return _MVPMatrix;
}

Effect* Renderable::GetEffect()
{
    return _Effect;
}

void Renderable::SetEffect(Effect* effect)
{
    _Effect = effect;
}

void Renderable::RefreshSystemBinding()
{
    if (_System)
    {
        RenderSystem* system = _System;
        system->RemoveItem(this);
        system->AddItem(this);
    }
}
