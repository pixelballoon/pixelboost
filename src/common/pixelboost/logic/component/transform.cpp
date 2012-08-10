#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

TransformComponent::TransformComponent(Entity* parent, Uid parentTransform)
    : Component(parent)
{
    
}

TransformComponent::~TransformComponent()
{
    
}

Uid TransformComponent::GetType()
{
    return GetStaticType();
}

Uid TransformComponent::GetStaticType()
{
    return TypeHash("pb::TransformComponent");
}
