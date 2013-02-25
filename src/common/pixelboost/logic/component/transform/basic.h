#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/transform.h"

namespace pb
{
    
    class BasicTransformComponent : public TransformComponent
    {
        PB_DECLARE_COMPONENT
        
    public:
        BasicTransformComponent(Entity* parent, Uid parentTransform = 0);
        virtual ~BasicTransformComponent();
        
        const glm::mat4x4& GetMatrix();
        
        void SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
        
        glm::vec3 GetPosition();
        void SetPosition(const glm::vec3& position);
        
        glm::vec3 GetRotation();
        void SetRotation(const glm::vec3& rotation);
        
        glm::vec3 GetScale();
        void SetScale(const glm::vec3& scale);
        
    private:
        void OnChanged();
        
        bool _Dirty;
        glm::mat4x4 _Matrix;
        glm::vec3 _Position;
        glm::vec3 _Rotation;
        glm::vec3 _Scale;
    };
    
}
