#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/logic/component.h"

namespace pb
{

class Message;
class SpriteRenderable;

class SpriteComponent : public Component
{
    PB_DECLARE_COMPONENT
    
protected:
    SpriteComponent(Entity* parent);
    virtual ~SpriteComponent();
    
public:
    glm::vec2 GetSize();
    
    void SetRenderPass(RenderPass renderPass);
    void SetLayer(int layer);
    void SetSprite(const std::string& sprite);
    void SetTint(const glm::vec4& tint);
    
    void SetLocalTransform(const glm::mat4x4& transform);
    
private:
    void OnTransformChanged(const Message& message);
    void UpdateTransform();
    
    glm::mat4x4 _LocalTransform;
    SpriteRenderable* _Renderable;
};
    
}
