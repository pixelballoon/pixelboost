#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"

#include "project/entity.h"
#include "view/entity/property/property.h"
#include "view/entity/entity.h"
#include "view.h"

using namespace pixeleditor;

ViewEntity::ViewEntity(Entity* entity)
    : _Entity(entity)
{
    
}

ViewEntity::~ViewEntity()
{
    
}

void ViewEntity::Update(float time)
{
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->Update(time);
    }
}

void ViewEntity::Render(pb::RenderLayer* layer)
{
    Vec3 position = _Entity->GetPosition();
    
    View::Instance()->GetPrimitiveRenderer()->AttachBox(layer, Vec2(position[0], position[1]), Vec2(0.1,0.1));
    
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->Render(layer);
    }
}

void ViewEntity::ParseProperties()
{
    
}
