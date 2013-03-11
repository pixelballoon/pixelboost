#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
    class GuiComponent;
    class GuiRenderSystem;
    
    class GuiRenderMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        GuiRenderMessage(GuiRenderSystem* renderSystem, GuiComponent* guiComponent);
        virtual ~GuiRenderMessage();
        
        GuiRenderSystem* GetGuiRenderSystem() const;
        GuiComponent* GetGuiComponent() const;
        
    private:
        GuiRenderSystem* _GuiRenderSystem;
        GuiComponent* _GuiComponent;
    };
    
}
