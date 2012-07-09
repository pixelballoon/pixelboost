#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

using namespace pb;

Renderable::Renderable(Uid entityUid)
    : _Layer(0)
    , _Effect(0)
    , _EntityUid(entityUid)
    , _WorldMatrixDirty(true)
{
    
}

Renderable::Renderable(Uid entityUid, Effect* effect)
    : _Layer(0)
    , _Effect(effect)
    , _EntityUid(entityUid)
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

void Renderable::SetLayer(int layer)
{
    _Layer = layer;
}

int Renderable::GetLayer()
{
    return _Layer;
}

void Renderable::DirtyWorldMatrix()
{
    _WorldMatrixDirty = true;
}

const glm::mat4x4& Renderable::GetWorldMatrix()
{
    if (_WorldMatrixDirty)
    {
        CalculateWorldMatrix();
        _WorldMatrixDirty = false;
    }
    
    return _WorldMatrix;
}

void Renderable::CalculateMVP(Viewport* viewport)
{
    if (_WorldMatrixDirty)
    {
        CalculateWorldMatrix();
        _WorldMatrixDirty = false;
    }
    
    _MVPMatrix = viewport->GetCamera()->ViewProjectionMatrix * _WorldMatrix;
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
