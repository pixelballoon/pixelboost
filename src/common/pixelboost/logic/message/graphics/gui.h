#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
    class GuiRenderSystem;
    
    class GuiRenderMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        GuiRenderMessage(GuiRenderSystem* renderSystem);
        virtual ~GuiRenderMessage();
        
        GuiRenderSystem* GetGuiRenderSystem() const;
        
    private:
        GuiRenderSystem* _GuiRenderSystem;
    };
    
}
