#include "pixelboost/logic/component/bounds.h"

using namespace pb;

BoundsComponent::BoundsComponent(Entity* parent)
    : Component(parent)
{
    
}

BoundsComponent::~BoundsComponent()
{
    
}

Uid BoundsComponent::GetType()
{
    return TypeHash("bounds");
}
