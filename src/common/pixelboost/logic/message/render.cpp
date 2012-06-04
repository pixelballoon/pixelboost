#include "pixelboost/logic/message/render.h"

using namespace pb;

RenderMessage::RenderMessage(Uid source, Viewport* viewport)
    : Message(source)
    , _Viewport(viewport)
{
    
}

RenderMessage::~RenderMessage()
{
    
}

Uid RenderMessage::GetType()
{
    return TypeHash("render");
}

Viewport* RenderMessage::GetViewport()
{
    return _Viewport;
}
