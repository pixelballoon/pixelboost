#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/message.h"

namespace pb
{
    
    class SetPositionMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        SetPositionMessage(glm::vec3 position);
        
        glm::vec3 GetPosition();
        
    private:
        glm::vec3 _Position;
    };
    
    class SetRotationMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        SetRotationMessage(glm::vec3 rotation);
        
        glm::vec3 GetRotation();
        
    private:
        glm::vec3 _Rotation;
    };
    
    class SetScaleMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        SetScaleMessage(glm::vec3 scale);
        
        glm::vec3 GetScale();
        
    private:
        glm::vec3 _Scale;
    };
    
    class TransformChangedMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        TransformChangedMessage(Entity* entity, Component* component);
        virtual ~TransformChangedMessage();
        
    private:
    };
    
}
