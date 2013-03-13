#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/graphics/renderable.h"
#include "pixelboost/logic/message/graphics/gui.h"

namespace pb
{
    
    class GuiRenderable;
    class GuiSystem;
    class Message;
    
    struct GuiInputEvent;
    struct GuiState;
    
    class GuiComponent : public RenderableComponent<GuiRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        GuiComponent(Entity* parent);
        virtual ~GuiComponent();
        
    public:
        void SetSize(glm::vec2 size);
        glm::vec2 GetSize();
        
    private:
        void OnGui(GuiState& state, GuiSystem* system, GuiRenderMessage::EventType eventType);
        
        glm::vec2 _Size;
        
        friend class GuiSystem;
    };
    
}
