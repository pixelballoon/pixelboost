#pragma once

#include <string>

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
    
    Uid GetType();
    
    void SetColor(glm::vec4 color);
    glm::vec4 GetColor();
    
    void SetSize(glm::vec2 size);
    glm::vec2 GetSize();
    
private:
    PrimitiveRenderableRectangle* _Renderable;
};
    
}
