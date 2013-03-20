#pragma once

#include "sigslot/signal.h"

namespace pb
{
    class Message;
    
    typedef sigslot::Delegate1<const Message&> MessageHandler;
}
