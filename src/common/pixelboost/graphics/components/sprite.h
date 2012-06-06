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
public:
    SpriteComponent(Entity* parent, const std::string& sprite);
    virtual ~SpriteComponent();
    
    Uid GetType();
    static Uid GetStaticType();
    
    glm::vec2 GetSize();
    
    void SetLayer(int layer);
    void SetSprite(const std::string& sprite);
    void SetTint(const glm::vec4& tint);
    
private:
    void OnUpdate(Uid sender, Message& message);
    
    SpriteRenderable* _Renderable;
};
    
}
