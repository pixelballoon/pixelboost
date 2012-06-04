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
    _Layer = 0;
}

int Renderable::GetLayer()
{
    return _Layer;
}


Effect* Renderable::GetEffect()
{
    return _Effect;
}
