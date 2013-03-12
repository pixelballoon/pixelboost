#include "pixelboost/logic/message/graphics/gui.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::GuiRenderMessage)

GuiRenderMessage::GuiRenderMessage(GuiGlobalState& state, GuiRenderSystem* renderSystem, GuiComponent* guiComponent, EventType eventType, GuiInputEvent inputEvent)
    : Message(0,0)
    , _State(state)
    , _GuiRenderSystem(renderSystem)
    , _GuiComponent(guiComponent)
    , _EventType(eventType)
    , _InputEvent(inputEvent)
{
    
}

GuiRenderMessage::~GuiRenderMessage()
{
    
}

GuiGlobalState& GuiRenderMessage::GetState() const
{
    return _State;
}

GuiRenderSystem* GuiRenderMessage::GetGuiRenderSystem() const
{
    return _GuiRenderSystem;
}

GuiComponent* GuiRenderMessage::GetGuiComponent() const
{
    return _GuiComponent;
}

GuiRenderMessage::EventType GuiRenderMessage::GetEventType() const
{
    return _EventType;
}

GuiInputEvent GuiRenderMessage::GetInputEvent() const
{
    return _InputEvent;
}
