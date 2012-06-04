#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
class Viewport;

class RenderMessage : public Message
{
public:
    RenderMessage(Uid source, Viewport* viewport);
    ~RenderMessage();
    
    Uid GetType();
    
    Viewport* GetViewport();
    
private:
    Viewport* _Viewport;
};

}
