#include "pixelboost/logic/message/graphics/gui.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::GuiRenderMessage)

GuiRenderMessage::GuiRenderMessage(GuiState& state, GuiSystem* system, GuiComponent* guiComponent, EventType eventType, GuiInputEvent inputEvent)
    : Message(0,0)
    , _State(state)
    , _GuiSystem(system)
    , _GuiComponent(guiComponent)
    , _EventType(eventType)
    , _InputEvent(inputEvent)
{
    
}

GuiRenderMessage::~GuiRenderMessage()
{
    
}

GuiState& GuiRenderMessage::GetState() const
{
    return _State;
}

GuiSystem* GuiRenderMessage::GetGuiSystem() const
{
    return _GuiSystem;
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
