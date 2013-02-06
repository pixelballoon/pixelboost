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

PhysicsCollisionStartMessage::PhysicsCollisionStartMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal)
    : PhysicsCollisionMessage(other, position, normal)
{
    
}

PhysicsCollisionStartMessage::~PhysicsCollisionStartMessage()
{
    
}

Uid PhysicsCollisionStartMessage::GetType() const
{
    return PhysicsCollisionStartMessage::GetStaticType();
}

Uid PhysicsCollisionStartMessage::GetStaticType()
{
    return TypeHash("pb::PhysicsCollisionStartMessage");
}

PhysicsCollisionEndMessage::PhysicsCollisionEndMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal)
    : PhysicsCollisionMessage(other, position, normal)
{
    
}

PhysicsCollisionEndMessage::~PhysicsCollisionEndMessage()
{
    
}

Uid PhysicsCollisionEndMessage::GetType() const
{
    return PhysicsCollisionEndMessage::GetStaticType();
}

Uid PhysicsCollisionEndMessage::GetStaticType()
{
    return TypeHash("pb::PhysicsCollisionEndMessage");
}
