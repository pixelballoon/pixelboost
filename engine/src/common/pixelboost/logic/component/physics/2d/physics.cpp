#include "Box2D/Box2D.h"

#include "pixelboost/logic/component/physics/2d/physics.h"

using namespace pb;

PhysicsComponent2D::PhysicsComponent2D(Entity* parent)
    : PhysicsComponent(parent)
{
    
}

PhysicsComponent2D::~PhysicsComponent2D()
{
    
}

b2Body* PhysicsComponent2D::GetBody()
{
    return _Body;
}

void PhysicsComponent2D::SetSensor(bool isSensor)
{
    b2Fixture* fixture = _Body->GetFixtureList();
    
    while (fixture)
    {
        fixture->SetSensor(isSensor);
        fixture = fixture->GetNext();
    }
}
