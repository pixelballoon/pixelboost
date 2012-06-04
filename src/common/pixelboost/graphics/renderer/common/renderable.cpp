#include "pixelboost/graphics/renderer/common/renderable.h"

using namespace pb;

Renderable::Renderable()
    : _Layer(0)
    , _Material(0)
{
    
}

Renderable::Renderable(Material* material)
    : _Layer(0)
    , _Material(material)
{
    
}

Renderable::~Renderable()
{
    
}

void Renderable::SetLayer(int layer)
{
    _Layer = 0;
}

int Renderable::GetLayer()
{
    return _Layer;
}


Material* Renderable::GetMaterial()
{
    return _Material;
}
