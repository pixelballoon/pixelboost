#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

Component::Component(Entity* parent)
    : _Parent(parent)
{
    
}

Component::~Component()
{

}

Entity* Component::GetParent()
{
    return _Parent;
}
