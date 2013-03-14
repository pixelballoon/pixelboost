#include "pixelboost/debug/log.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/gui/controls.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"

using namespace pb;

void GuiControls::BeginArea(const GuiRenderMessage& message, GuiId guiId, const std::vector<GuiLayoutHint>& hints)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        GuiLayout area("area");
        area.LayoutType = GuiLayout::kLayoutTypeVertical;
        message.GetGuiSystem()->PushLayoutArea(area, guiId, hints);
        return;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
        
        if (!layout)
        {
            return;
        }
        
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        renderable->SetScissor(true, glm::vec4(0,0,layout->Size.x, layout->Size.y));
        
        renderable->RenderBoxFilled(layout->Position, layout->Size, glm::vec4(95.f/255.f,95.f/255.f,95.f/255.f,1));
    }
}

void GuiControls::EndArea(const GuiRenderMessage& message)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->PopLayoutArea();
        return;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        renderable->SetScissor(false);
    }
}

void GuiControls::BeginHorizontal(const GuiRenderMessage& message, GuiId guiId, const std::vector<GuiLayoutHint>& hints)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        GuiLayout area("horizontal");
        area.LayoutType = GuiLayout::kLayoutTypeHorizontal;
        message.GetGuiSystem()->PushLayoutArea(area, guiId, hints);
        return;
    }
}

void GuiControls::EndHorizontal(const GuiRenderMessage& message)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->PopLayoutArea();
        return;
    }
}

void GuiControls::BeginVertical(const GuiRenderMessage& message, GuiId guiId, const std::vector<GuiLayoutHint>& hints)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        GuiLayout area("vertical");
        area.LayoutType = GuiLayout::kLayoutTypeVertical;
        message.GetGuiSystem()->PushLayoutArea(area, guiId, hints);
        return;
    }
}

void GuiControls::EndVertical(const GuiRenderMessage& message)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->PopLayoutArea();
        return;
    }
}

void GuiControls::BeginScrollArea(const GuiRenderMessage& message, GuiId guiId, const std::vector<GuiLayoutHint>& hints)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        GuiData& data = message.GetGuiSystem()->GetData(guiId);
        
        if (!data.Initialised)
        {
            data.Initialised = true;
            data.Data[0].Float = 0.f;
            data.Data[1].Float = 0.f;
        }
        
        GuiLayout area("scrollarea");
        area.LayoutType = GuiLayout::kLayoutTypeVertical;
        area.Scroll.x = data.Data[0].Float;
        area.Scroll.y = data.Data[1].Float;
        message.GetGuiSystem()->PushLayoutArea(area, guiId, hints);
        return;
    }
    
    GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
    
    if (!layout)
    {
        return;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeInput)
    {
        GuiData& data = message.GetGuiSystem()->GetData(guiId);
        
        data.Data[0].Float = glm::clamp(data.Data[0].Float+message.GetState().MouseWheel.x, 0.f, layout->ContentsSize.x - layout->Size.x);
        data.Data[1].Float = glm::clamp(data.Data[1].Float-message.GetState().MouseWheel.y, 0.f, layout->ContentsSize.y - layout->Size.y + 50.f);
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        renderable->SetScissor(true, glm::vec4(0,0,layout->Size.x, layout->Size.y));
        
        renderable->RenderBoxFilled(layout->Position, layout->Size, glm::vec4(95.f/255.f,95.f/255.f,95.f/255.f,1));
    }
}

void GuiControls::EndScrollArea(const GuiRenderMessage& message)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->PopLayoutArea();
        return;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        renderable->SetScissor(false);
    }
}

std::string GuiControls::DoCombo(const GuiRenderMessage& message, GuiId guiId, const std::string& label, const std::vector<std::string>& options, const std::vector<GuiLayoutHint>& layout)
{
    return "";
}

bool GuiControls::DoButton(const GuiRenderMessage& message, GuiId guiId, const std::string& caption, const std::vector<GuiLayoutHint>& hints)
{
    GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        glm::vec2 textSize = renderable->MeasureText(message.GetState().Skin.Font, caption, 20.f);
        message.GetGuiSystem()->AddLayout("button: " + caption, guiId, hints, glm::vec2(textSize.x, 20.f));
        return false;
    }
    
    GuiState& state = message.GetState();
    GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
    
    if (!layout)
    {
        return false;
    }

    if (message.GetEventType() == GuiRenderMessage::kEventTypeInput)
    {
        bool pressed = false;
        
        glm::vec2 mousePosition = state.MousePosition;
        
        if (mousePosition.x > layout->Position.x &&
            mousePosition.x < layout->Position.x + layout->Size.x &&
            mousePosition.y > layout->Position.y &&
            mousePosition.y < layout->Position.y + layout->Size.y)
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
        
        return pressed;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        glm::vec4 color;

        if (state.Active.Item == guiId)
        {
            color = glm::vec4(86.f/255.f,86.f/255.f,86.f/255.f,1);
        } else if (state.Hot.Item == guiId)
        {
            color = glm::vec4(166.f/255.f,166.f/255.f,166.f/255.f,1);
        } else
        {
            color = glm::vec4(0.5,0.5,0.5,1);
        }
        
        renderable->RenderBoxFilled(layout->Position, layout->Size, color);
        
        renderable->RenderBoxOutline(layout->Position, layout->Size, glm::vec4(0.2,0.2,0.2,1));
        
        renderable->RenderText(layout->Position, message.GetState().Skin.Font, caption, 20.f, glm::vec4(0,0,0,1));
    }
    
    return false;
}
