#pragma once

#include <string>

#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class Message;
    class PrimitiveRenderableLine;
        
    class LineComponent : public RenderableComponent<PrimitiveRenderableLine>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        LineComponent(Entity* parent);
        virtual ~LineComponent();
        
    public:
        void SetColor(glm::vec4 color);
        glm::vec4 GetColor();
        
        void SetLine(glm::vec3 start, glm::vec3 end);
    };
    
}
