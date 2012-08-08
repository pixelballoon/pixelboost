#include "pixelboost/logic/message/debug/render.h"

using namespace pb;

DebugRenderMessage::DebugRenderMessage(DebugRenderSystem* renderSystem)
    : Message(0,0)
    , _DebugRenderSystem(renderSystem)
{
    
}

DebugRenderMessage::~DebugRenderMessage()
{
    
}

Uid DebugRenderMessage::GetType() const
{
    return GetStaticType();
}

Uid DebugRenderMessage::GetStaticType()
{
    return TypeHash("pb::DebugRenderMessage");
}

DebugRenderSystem* DebugRenderMessage::GetDebugRenderSystem() const
{
    return _DebugRenderSystem;
}