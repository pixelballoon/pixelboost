#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/transform.h"

namespace pb
{
    
    class Message;
    
    class ParallaxTransformComponent : public TransformComponent
    {
        PB_DECLARE_COMPONENT
        
    public:
        ParallaxTransformComponent(Entity* parent, Uid parallaxEntityId, Uid parentTransform = 0);
        virtual ~ParallaxTransformComponent();
        
        const glm::mat4x4& GetMatrix();
        
        void SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
        
        glm::vec3 GetPosition();
        void SetPosition(const glm::vec3& position);
        
        glm::vec3 GetRotation();
        void SetRotation(const glm::vec3& rotation);
        
        glm::vec3 GetScale();
        void SetScale(const glm::vec3& scale);
        
        glm::vec3 GetParallaxScale();
        void SetParallaxScale(const glm::vec3& parallaxScale);
        
    private:
        void OnChanged();
        void OnTransformChanged(const Message& message);
        
        Uid _ParallaxEntityId;
        
        bool _Dirty;
        glm::mat4x4 _Matrix;
        glm::vec3 _Position;
        glm::vec3 _Rotation;
        glm::vec3 _Scale;
        
        glm::vec3 _ParallaxScale;
        glm::vec3 _ParallaxPosition;
    };
    
}
