#pragma once

#include "glm/glm.hpp"
#include "pixelboost/logic/message.h"

namespace pb
{

    class PhysicsComponent;
    
    class PhysicsCollisionMessage : public pb::Message
    {
    public:
        PhysicsCollisionMessage(PhysicsComponent* other, glm::vec2 position, glm::vec2 normal);
        ~PhysicsCollisionMessage();
        
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        PhysicsComponent* GetOtherComponent() const;
        glm::vec2 GetPosition() const;
        glm::vec2 GetNormal() const;

    private:
        PhysicsComponent* _Other;
        glm::vec2 _Position;
        glm::vec2 _Normal;
    };
    
}