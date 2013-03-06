#include "pixelboost/logic/message/physics/collision.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::PhysicsCollisionMessage)

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

PB_DEFINE_MESSAGE(pb::PhysicsCollisionStartMessage)

PhysicsCollisionStartMessage::PhysicsCollisionStartMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal)
    : PhysicsCollisionMessage(other, position, normal)
{
    
}

PhysicsCollisionStartMessage::~PhysicsCollisionStartMessage()
{
    
}

PB_DEFINE_MESSAGE(pb::PhysicsCollisionEndMessage)

PhysicsCollisionEndMessage::PhysicsCollisionEndMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal)
    : PhysicsCollisionMessage(other, position, normal)
{
    
}

PhysicsCollisionEndMessage::~PhysicsCollisionEndMessage()
{
    
}
