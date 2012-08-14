#include "pixelboost/logic/message/physics/collision.h"

using namespace pb;

PhysicsCollisionMessage::PhysicsCollisionMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal)
    : pb::Message(0, 0)
    , _Other(other)
    , _Position(position)
    , _Normal(normal)
{
    
}

PhysicsCollisionMessage::~PhysicsCollisionMessage()
{
    
}

Uid PhysicsCollisionMessage::GetType() const
{
    return PhysicsCollisionMessage::GetStaticType();
}

Uid PhysicsCollisionMessage::GetStaticType()
{
    return TypeHash("pb::PhysicsCollisionMessage");
}

PhysicsComponent* PhysicsCollisionMessage::GetOtherComponent() const
{
    return _Other;
}

glm::vec2 PhysicsCollisionMessage::GetPosition() const
{
    return _Position;
}

glm::vec2 PhysicsCollisionMessage::GetNormal() const
{
    return _Normal;
}