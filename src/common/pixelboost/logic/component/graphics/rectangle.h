#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
class Message;
class PrimitiveRenderableRectangle;
    
class RectangleComponent : public Component
{
public:
    RectangleComponent(Entity* parent);
    virtual ~RectangleComponent();
    
    virtual Uid GetType();
    static Uid GetStaticType();    
    
    void SetRenderPass(RenderPass renderPass);
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
    PrimitiveRenderableRectangle* _Renderable;
};
    
}
