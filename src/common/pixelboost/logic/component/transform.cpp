#include "pixelboost/logic/component/transform.h"

using namespace pb;

TransformComponent::TransformComponent()
{
    
}

TransformComponent::~TransformComponent()
{
    
}

Uid TransformComponent::GetType()
{
    return TypeHash("transform");
}
