#pragma once

#include <string>

#include "pixelboost/logic/component.h"

namespace pb
{
    
class Message;
class PrimitiveRenderableEllipse;
    
class EllipseComponent : public Component
{
public:
    EllipseComponent(Entity* parent);
    virtual ~EllipseComponent();
    
    Uid GetType();
    
    void SetColor(glm::vec4 color);
    glm::vec4 GetColor();
    
    void SetSize(glm::vec2 size);
    glm::vec2 GetSize();
    
private:
    PrimitiveRenderableEllipse* _Renderable;
};
    
}
