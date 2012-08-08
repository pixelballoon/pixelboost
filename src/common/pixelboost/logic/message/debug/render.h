#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
    class DebugRenderSystem;
    
    class DebugRenderMessage : public Message
    {
    public:
        DebugRenderMessage(DebugRenderSystem* renderSystem);
        virtual ~DebugRenderMessage();
        
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        DebugRenderSystem* GetDebugRenderSystem() const;
        
    private:
        DebugRenderSystem* _DebugRenderSystem;
    };
    
}
