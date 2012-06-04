#include "pixelboost/logic/entity.h"

using namespace pb;

Entity::Entity(Uid uid)
    : _Uid(uid)
{
    
}

Entity::~Entity()
{
    
}

void Entity::AddComponent(Component* component)
{
    _Components.insert(component);
}

void Entity::RemoveComponent(Component* component)
{
    _Components.erase(component);
}
