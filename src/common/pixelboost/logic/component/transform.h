#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component.h"

namespace pb
{

class TransformComponent : public Component
{
public:
    TransformComponent(Entity* parent);
    virtual ~TransformComponent();
    
    Uid GetType();
    
    const glm::mat4x4& GetTransformMatrix();
    
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);
    
    
private:
    bool _Dirty;
    glm::mat4x4 _Matrix;
    glm::vec3 _Position;
    glm::vec3 _Rotation;
    glm::vec3 _Scale;
};

}
