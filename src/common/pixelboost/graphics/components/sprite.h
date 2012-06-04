#pragma once

#include <string>

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/logic/component.h"

namespace pb
{

class Message;
    
class SpriteComponent : public Component
{
public:
    SpriteComponent(Entity* parent, const std::string& sprite);
    virtual ~SpriteComponent();
    
    Uid GetType();
    
    virtual Material* GetMaterial();
    
private:
    void OnRender(Uid sender, Message& message);
    
    std::string _Sprite;
};
    
}
