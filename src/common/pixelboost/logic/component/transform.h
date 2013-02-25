#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component.h"

namespace pb
{

    class TransformComponent : public Component
    {
        PB_DECLARE_COMPONENT
        
    public:
        TransformComponent(Entity* parent, Uid parentTransform);
        virtual ~TransformComponent();
        
        virtual const glm::mat4x4& GetMatrix() = 0;
        
        virtual void SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) = 0;
        
        virtual glm::vec3 GetPosition() = 0;
        virtual void SetPosition(const glm::vec3& position) = 0;
        
        virtual glm::vec3 GetRotation() = 0;
        virtual void SetRotation(const glm::vec3& rotation) = 0;
        
        virtual glm::vec3 GetScale() = 0;
        virtual void SetScale(const glm::vec3& scale) = 0;
        
    private:
    };

}
