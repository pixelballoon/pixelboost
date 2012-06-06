#include "pixelboost/logic/component/physics/physics.h"

using namespace pb;

PhysicsComponent::PhysicsComponent(Entity* parent)
    : pb::Component(parent)
    , _UserData(0)
{
    
}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::SetUserData(void* userData)
{
    _UserData = userData;
}

void* PhysicsComponent::GetUserData()
{
    return _UserData;
}
