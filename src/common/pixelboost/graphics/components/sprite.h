#pragma once

#include <string>

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
    
private:
    void OnRender(Uid sender, Message& message);
    
    SpriteRenderable* _Renderable;
};
    
}
