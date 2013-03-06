#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

PB_DEFINE_COMPONENT_ABSTRACT(pb::TransformComponent)

TransformComponent::TransformComponent(Entity* parent)
    : Component(parent)
{
    
}

TransformComponent::~TransformComponent()
{
    
}

void TransformComponent::Dirty()
{
    
}
