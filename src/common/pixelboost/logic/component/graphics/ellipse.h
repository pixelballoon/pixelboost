#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
class Message;
class PrimitiveRenderableEllipse;
    
class EllipseComponent : public RenderableComponent<PrimitiveRenderableEllipse>
{
    PB_DECLARE_COMPONENT
    
protected:
    EllipseComponent(Entity* parent);
    virtual ~EllipseComponent();
    
public:
    void SetLayer(int layer);
    
    void SetColor(glm::vec4 color);
    glm::vec4 GetColor();
    
    void SetSize(glm::vec2 size);
    glm::vec2 GetSize();
    
    void SetSolid(bool solid);
    bool GetSolid();
};
    
}
