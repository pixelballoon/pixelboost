#include "pixelboost/logic/component/transform/parent.h"

using namespace pb;

ParentTransformComponent::ParentTransformComponent(Entity* parent, pb::Uid destinationEntity)
    : Component(parent)
{
    
}

ParentTransformComponent::~ParentTransformComponent()
{
    
}

void ParentTransformComponent::OnTransformChanged(Uid messageId, Message& message)
{
    
}
