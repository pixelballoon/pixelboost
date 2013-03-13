#pragma once

#include "pixelboost/logic/system/graphics/gui/gui.h"
#include "pixelboost/logic/message.h"

namespace pb
{
    
    class GuiComponent;
    
    class GuiRenderMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        enum EventType
        {
            kEventTypeLayout,
            kEventTypeInput,
            kEventTypeRender,
        };
        
    public:
        GuiRenderMessage(GuiState& state, GuiSystem* system, GuiComponent* guiComponent, EventType eventType, GuiInputEvent inputEvent = GuiInputEvent());
        virtual ~GuiRenderMessage();
        
        GuiState& GetState() const;
        
        GuiSystem* GetGuiSystem() const;
        GuiComponent* GetGuiComponent() const;
        
        EventType GetEventType() const;
        GuiInputEvent GetInputEvent() const;
        
    private:
        GuiState& _State;
        
        GuiSystem* _GuiSystem;
        GuiComponent* _GuiComponent;
        
        EventType _EventType;
        GuiInputEvent _InputEvent;
    };
    
}
