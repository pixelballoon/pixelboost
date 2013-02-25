#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/logic/component.h"

namespace pb
{
    
class Message;
class PrimitiveRenderableEllipse;
    
class EllipseComponent : public Component
{
    PB_DECLARE_COMPONENT
    
public:
    EllipseComponent(Entity* parent);
    virtual ~EllipseComponent();
    
    void SetLayer(int layer);
    
    void SetColor(glm::vec4 color);
    glm::vec4 GetColor();
    
    void SetSize(glm::vec2 size);
    glm::vec2 GetSize();
    
    void SetSolid(bool solid);
    bool GetSolid();
    
    void SetLocalTransform(const glm::mat4x4& transform);
    
private:
    void OnTransformChanged(const Message& message);
    void UpdateTransform();
    
private:
    glm::mat4x4 _LocalTransform;
    PrimitiveRenderableEllipse* _Renderable;
};
    
}
