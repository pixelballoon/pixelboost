#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
    class DebugRenderSystem;
    
    class DebugRenderMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        DebugRenderMessage(DebugRenderSystem* renderSystem);
        virtual ~DebugRenderMessage();
        
        DebugRenderSystem* GetDebugRenderSystem() const;
        
    private:
        DebugRenderSystem* _DebugRenderSystem;
    };
    
}
