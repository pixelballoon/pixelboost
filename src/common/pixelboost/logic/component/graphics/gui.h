#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class GuiRenderable;
    class GuiRenderSystem;
    class Message;
    
    class GuiComponent : public RenderableComponent<GuiRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        GuiComponent(Entity* parent);
        virtual ~GuiComponent();
        
        void SetSize(glm::vec2 size, glm::vec2 position);
        
    private:
        void Render(GuiRenderSystem* system);
        
        friend class GuiRenderSystem;
    };
    
}
