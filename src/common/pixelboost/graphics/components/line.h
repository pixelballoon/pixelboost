#pragma once

#include <string>

#include "pixelboost/logic/component.h"

namespace pb
{
    
class Message;
class PrimitiveRenderableLine;
    
class LineComponent : public Component
{
public:
    LineComponent(Entity* parent);
    virtual ~LineComponent();
    
    Uid GetType();
    
    void SetColor(glm::vec4 color);
    glm::vec4 GetColor();
    
    void SetLine(glm::vec3 start, glm::vec3 end);
    
private:
    void OnRender(Uid sender, Message& message);
    
    PrimitiveRenderableLine* _Renderable;
};
    
}
