#include "pixelboost/logic/component/physics/physics.h"

using namespace pb;

PB_DEFINE_COMPONENT(PhysicsComponent)

PhysicsComponent::PhysicsComponent(Entity* parent)
    : Component(parent)
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
