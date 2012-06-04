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
    
    virtual Uid GetType();
    static Uid GetStaticType();
    
    Viewport* GetViewport();
    
private:
    Viewport* _Viewport;
};

}
