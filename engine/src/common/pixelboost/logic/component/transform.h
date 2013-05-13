#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Message;

    class TransformComponent : public Component
    {
        PB_DECLARE_COMPONENT
        
    protected:
        TransformComponent(Entity* parent);
        virtual ~TransformComponent();
        
    public:
        const glm::mat4x4& GetMatrix();
        
        void SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
        
        glm::vec3 GetPosition();
        void SetPosition(const glm::vec3& position);
        
        glm::vec3 GetRotation();
        void SetRotation(const glm::vec3& rotation);
        
        glm::vec3 GetScale();
        void SetScale(const glm::vec3& scale);
        
        glm::vec3 GetWorldPosition();
        
        void Dirty(bool dirtyThis);
        
    private:
        void OnSetPosition(const Message& message);
        void OnSetRotation(const Message& message);
        void OnSetScale(const Message& message);
        
        enum DirtyType
        {
            kDirtyTypeNone,
            kDirtyTypeThis,
            kDirtyTypeParent,
        };
        
        DirtyType _Dirty;
        
        glm::mat4x4 _LocalMatrix;
        glm::mat4x4 _Matrix;
        glm::vec3 _Position;
        glm::vec3 _Rotation;
        glm::vec3 _Scale;
    };

}
