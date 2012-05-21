#include "view/entity/property/sprite.h"

using namespace pixeleditor;

SpriteViewProperty::SpriteViewProperty(ViewEntity* parent)
    : ViewProperty(parent)
{
    
}

SpriteViewProperty::~SpriteViewProperty()
{
    
}

void SpriteViewProperty::Update(float time)
{
    ViewProperty::Update(time);
}

void SpriteViewProperty::Render(pb::RenderLayer* layer)
{
    ViewProperty::Render(layer);
}
