#include "pixelboost/logic/message/debug/render.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::DebugRenderMessage)

DebugRenderMessage::DebugRenderMessage(DebugRenderSystem* renderSystem)
    : Message(0,0)
    , _DebugRenderSystem(renderSystem)
{
    
}

DebugRenderMessage::~DebugRenderMessage()
{
    
}

DebugRenderSystem* DebugRenderMessage::GetDebugRenderSystem() const
{
    return _DebugRenderSystem;
}
