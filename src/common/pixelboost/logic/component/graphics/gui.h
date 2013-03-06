#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class GuiRenderable;
    class Message;
    
    class GuiComponent : public RenderableComponent<GuiRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        GuiComponent(Entity* parent);
        virtual ~GuiComponent();
    };
    
}
