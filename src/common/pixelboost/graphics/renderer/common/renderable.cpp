#include "pixelboost/graphics/renderer/common/renderable.h"

using namespace pb;

Renderable::Renderable(Uid entityUid)
    : _Layer(0)
    , _Effect(0)
    , _EntityUid(entityUid)
{
    
}

Renderable::Renderable(Uid entityUid, Effect* effect)
    : _Layer(0)
    , _Effect(effect)
    , _EntityUid(entityUid)
{
    
}

Renderable::~Renderable()
{
    
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

const glm::mat4x4& Renderable::GetMVP() const
{
    return _MVPMatrix;
}

Effect* Renderable::GetEffect()
{
    return _Effect;
}
