#pragma once

#include <string>

#include "pixelboost/logic/component.h"

namespace pb
{
    
class Message;
class PrimitiveRenderableLine;
    
class LineComponent : public Component
{
    PB_DECLARE_COMPONENT
    
public:
    LineComponent(Entity* parent);
    virtual ~LineComponent();
    
    void SetColor(glm::vec4 color);
    glm::vec4 GetColor();
    
    void SetLine(glm::vec3 start, glm::vec3 end);
    
    void SetLocalTransform(const glm::mat4x4& localTransform);
    
private:
    void OnTransformChanged(const Message& message);
    void UpdateTransform();
    
private:
    glm::mat4x4 _LocalTransform;
    PrimitiveRenderableLine* _Renderable;
};
    
}
