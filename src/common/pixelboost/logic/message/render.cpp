#include "pixelboost/logic/message/render.h"

using namespace pb;

RenderMessage::RenderMessage(Viewport* viewport)
    : Message(0, 0)
    , _Viewport(viewport)
{
    
}

RenderMessage::~RenderMessage()
{
    
}

Uid RenderMessage::GetType()
{
    return GetStaticType();
}

Uid RenderMessage::GetStaticType()
{
    return TypeHash("render");
}

Viewport* RenderMessage::GetViewport()
{
    return _Viewport;
}
