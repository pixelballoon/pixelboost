#include "pixelboost/logic/message/graphics/gui.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::GuiRenderMessage)

GuiRenderMessage::GuiRenderMessage(GuiRenderSystem* renderSystem)
    : Message(0,0)
    , _GuiRenderSystem(renderSystem)
{
    
}

GuiRenderMessage::~GuiRenderMessage()
{
    
}

GuiRenderSystem* GuiRenderMessage::GetGuiRenderSystem() const
{
    return _GuiRenderSystem;
}
