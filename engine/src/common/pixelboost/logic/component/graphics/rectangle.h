#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class Message;
    class PrimitiveRenderableRectangle;
        
    class RectangleComponent : public RenderableComponent<PrimitiveRenderableRectangle>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        RectangleComponent(Entity* parent);
        virtual ~RectangleComponent();
        
    public:
        void SetColor(glm::vec4 color);
        glm::vec4 GetColor();
        
        void SetSize(glm::vec2 size);
        glm::vec2 GetSize();
        
        void SetSolid(bool solid);
        bool GetSolid();
    };
    
}
