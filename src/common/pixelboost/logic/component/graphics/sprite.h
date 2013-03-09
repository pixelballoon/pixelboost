#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{

class Message;

class SpriteComponent : public RenderableComponent<SpriteRenderable>
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
};
    
}
