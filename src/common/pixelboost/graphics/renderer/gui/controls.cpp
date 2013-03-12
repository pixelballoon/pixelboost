#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/renderer/gui/controls.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"

using namespace pb;

void GuiControls::BeginArea(const GuiRenderMessage& message, const std::vector<GuiLayoutHint>& hints)
{

}

void GuiControls::EndArea(const GuiRenderMessage& message)
{
    
}

void GuiControls::BeginHorizontal(const GuiRenderMessage& message, const std::vector<GuiLayoutHint>& hints)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        GuiLayoutArea area;
        area.Type = GuiLayoutArea::kLayoutTypeHorizontal;
        area.Position = message.GetState().LayoutStack.top().Pointer;
        area.Pointer = area.Position;
        message.GetGuiRenderSystem()->PushLayoutArea(area);
        return;
    }
}

void GuiControls::EndHorizontal(const GuiRenderMessage& message)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiRenderSystem()->PopLayoutArea();
        return;
    }
}

void GuiControls::BeginScrollArea(const GuiRenderMessage& message, const std::vector<GuiLayoutHint>& hints)
{

}

void GuiControls::EndScrollArea(const GuiRenderMessage& message)
{
    
}

std::string GuiControls::DoCombo(const GuiRenderMessage& message, GuiId guiId, const std::string& label, const std::vector<std::string>& options, const std::vector<GuiLayoutHint>& layout)
{
    return "";
}

bool GuiControls::DoButton(const GuiRenderMessage& message, GuiId guiId, const std::string& caption, const std::vector<GuiLayoutHint>& hints)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiRenderSystem()->AddLayout(guiId, hints, glm::vec2(128,32));
        return false;
    }
    
    GuiLayout layout = message.GetGuiRenderSystem()->GetLayout(guiId);

    GuiState& state = message.GetState();
    
    bool pressed = false;
    
    glm::vec2 mousePosition = state.MousePosition;
    
    if (mousePosition.x > layout.Position.x &&
        mousePosition.x < layout.Position.x + layout.Size.x &&
        mousePosition.y > layout.Position.y &&
        mousePosition.y < layout.Position.y + layout.Size.y)
    {
        if (!state.Hot.Item)
        {
            state.Hot.Item = guiId;
        }
        
        if (!state.Active.Item && state.MousePressed)
        {
            state.Active.Item = guiId;
        }
    }
    
    if (state.Active.Item == guiId)
    {
        if (!state.MouseDown || state.MouseReleased)
        {
            if (state.Hot.Item == guiId)
            {
                pressed = true;
            }
        } else {
            state.Active.Active = true;
        }
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        if (state.Active.Item == guiId)
        {
            renderable->RenderBoxOutline(layout.Position + glm::vec2(5,5), layout.Size - glm::vec2(10,10), glm::vec4(1.f,0.1,0.1,1));
        }
        
        renderable->RenderBoxOutline(layout.Position, layout.Size, (message.GetState().Hot.Item == guiId) ? glm::vec4(1,0,1,1) : glm::vec4(0.5,0.5,0.5,1));
        
        renderable->RenderText(layout.Position, "Test", glm::vec4(1,1,1,1));
    }
    
    return pressed;
}
