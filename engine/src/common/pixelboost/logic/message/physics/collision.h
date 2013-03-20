#pragma once

#include "glm/glm.hpp"
#include "pixelboost/logic/message.h"

namespace pb
{

    class PhysicsComponent;
    
    class PhysicsCollisionMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        PhysicsCollisionMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal);
        ~PhysicsCollisionMessage();
        
        PhysicsComponent* GetOtherComponent() const;
        glm::vec2 GetPosition() const;
        glm::vec2 GetNormal() const;
        
    protected:
        PhysicsComponent* _Other;
        glm::vec2 _Position;
        glm::vec2 _Normal;
    };
    
    class PhysicsCollisionStartMessage : public PhysicsCollisionMessage
    {
        PB_DECLARE_MESSAGE
        
    public:
        PhysicsCollisionStartMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal);
        ~PhysicsCollisionStartMessage();
    };
    
    class PhysicsCollisionEndMessage : public PhysicsCollisionMessage
    {
        PB_DECLARE_MESSAGE
        
    public:
        PhysicsCollisionEndMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal);
        ~PhysicsCollisionEndMessage();
    };
    
}
