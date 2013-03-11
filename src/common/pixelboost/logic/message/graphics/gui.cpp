#include "pixelboost/logic/message/graphics/gui.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::GuiRenderMessage)

GuiRenderMessage::GuiRenderMessage(GuiRenderSystem* renderSystem, GuiComponent* guiComponent)
    : Message(0,0)
    , _GuiRenderSystem(renderSystem)
    , _GuiComponent(guiComponent)
{
    
}

GuiRenderMessage::~GuiRenderMessage()
{
    
}

GuiRenderSystem* GuiRenderMessage::GetGuiRenderSystem() const
{
    return _GuiRenderSystem;
}

GuiComponent* GuiRenderMessage::GetGuiComponent() const
{
    return _GuiComponent;
}
