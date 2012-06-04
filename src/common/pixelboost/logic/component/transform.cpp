#include "pixelboost/logic/component/transform.h"

using namespace pb;

TransformComponent::TransformComponent(Entity* parent)
    : Component(parent)
{
    
}

TransformComponent::~TransformComponent()
{
    
}

Uid TransformComponent::GetType()
{
    return TypeHash("transform");
}
