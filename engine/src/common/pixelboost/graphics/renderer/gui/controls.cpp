#include "pixelboost/debug/log.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/gui/controls.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"

using namespace pb;

const GuiId& GuiControls::BeginArea(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<GuiLayoutHint>& hints)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        GuiLayout area("area");
        area.LayoutType = GuiLayout::kLayoutTypeVertical;
        message.GetGuiSystem()->PushLayoutArea(area, guiId, hints);
        return guiId;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
        
        if (!layout)
        {
            return guiId;
        }
        
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        //renderable->SetScissor(true, glm::vec4(layout->Position.x, layout->Position.y+layout->Size.y-message.GetGuiComponent()->GetSize().y, layout->Size.x, layout->Size.y));
        
        renderable->RenderBoxFilled(layout->Position, layout->Size, glm::vec4(95.f/255.f,95.f/255.f,95.f/255.f,1));
    }
    
    return guiId;
}

void GuiControls::EndArea(const GuiRenderMessage& message, const GuiId& guiId)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->PopLayoutArea();
        return;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeInput)
    {
        GuiState& state = message.GetState();
        GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
        
        if (!layout)
        {
            return;
        }
        
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
        }
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        renderable->SetScissor(false);
    }
}

void GuiControls::BeginHorizontal(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<GuiLayoutHint>& hints)
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

void GuiControls::BeginVertical(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<GuiLayoutHint>& hints)
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

const GuiId& GuiControls::BeginScrollArea(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<GuiLayoutHint>& hints)
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
        return guiId;
    }
    
    GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
    
    if (!layout)
    {
        return guiId;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeInput)
    {
        GuiState& state = message.GetState();
        GuiData& data = message.GetGuiSystem()->GetData(guiId);
        
        glm::vec2 mousePosition = state.MousePosition;
        
        if (mousePosition.x > layout->Position.x &&
            mousePosition.x < layout->Position.x + layout->Size.x &&
            mousePosition.y > layout->Position.y &&
            mousePosition.y < layout->Position.y + layout->Size.y)
        {
            data.Data[0].Float = glm::clamp(data.Data[0].Float+message.GetState().MouseWheel.x, 0.f, layout->ContentsSize.x - layout->Size.x);
            data.Data[1].Float = glm::clamp(data.Data[1].Float-message.GetState().MouseWheel.y, 0.f, layout->ContentsSize.y - layout->Size.y + 50.f);
        }
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        renderable->SetScissor(true, glm::vec4(layout->Position.x, layout->Position.y+layout->Size.y-message.GetGuiComponent()->GetSize().y, layout->Size.x, layout->Size.y));
        
        renderable->RenderBoxFilled(layout->Position, layout->Size, glm::vec4(95.f/255.f,95.f/255.f,95.f/255.f,1));
    }
    
    return guiId;
}

void GuiControls::EndScrollArea(const GuiRenderMessage& message, const GuiId& guiId)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->PopLayoutArea();
        return;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeInput)
    {
        GuiState& state = message.GetState();
        GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
        
        if (!layout)
        {
            return;
        }
        
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
        }
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
        
        renderable->SetScissor(false);
    }
}

void GuiControls::DoSpacer(const GuiRenderMessage& message, const GuiId& guiId, glm::vec2 minimumSize, const std::vector<GuiLayoutHint>& hints)
{
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->AddLayout("spacer", guiId, hints, minimumSize);
    }
}

int GuiControls::DoPageSelector(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<std::string>& options, const std::vector<GuiLayoutHint>& layout)
{
    GuiData& data = message.GetGuiSystem()->GetData(guiId);
    
    if (!data.Initialised)
    {
        data.Initialised = true;
        data.Data[0].Integer = -1;
    }
    
    if (data.Data[0].Integer == -1)
    {
        for (int i=0; i<options.size(); i++)
        {
            if (DoButton(message, PbNestedGuiId(guiId, i), options[i]))
            {
                data.Data[0].Integer = i;
            }
        }
    } else {
        if (DoButton(message, PbNestedGuiId(guiId, 0), "<-- Back"))
        {
            data.Data[0].Integer = -1;
        }
    }
    
    return data.Data[0].Integer;
}

int GuiControls::DoCombo(const GuiRenderMessage& message, const GuiId& guiId, const std::string& label, const std::vector<std::string>& options, const std::vector<GuiLayoutHint>& layout)
{
    int returnVal = -1;
    
    GuiData& data = message.GetGuiSystem()->GetData(guiId);
    
    if (!data.Initialised)
    {
        data.Initialised = true;
        data.Data[0].Bool = false;
    }
    
    if (DoButton(message, PbNestedGuiId(guiId, 0), label))
    {
        data.Data[0].Bool = !data.Data[0].Bool;
    }
    
    if (data.Data[0].Bool)
    {
        for (int i=0; i<options.size(); i++)
        {
            if (DoButton(message, PbNestedGuiId(guiId, i), options[i]))
            {
                returnVal = i;
                data.Data[0].Bool = false;
            }
        }
    }
    
    return returnVal;
}

void GuiControls::DoLabel(const GuiRenderMessage& message, const GuiId& guiId, const std::string& label, const std::vector<GuiLayoutHint>& hints)
{
    GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        glm::vec2 textSize = renderable->MeasureText(message.GetState().Skin.Font, label, 16.f);
        message.GetGuiSystem()->AddLayout("label: " + label, guiId, hints, glm::vec2(textSize.x, 20.f));
        return;
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
        
        if (!layout)
        {
            return;
        }
        
        renderable->RenderText(layout->Position + glm::vec2(2,0), message.GetState().Skin.Font, label, 16.f, glm::vec4(0,0,0,1));
    }
}

bool GuiControls::DoButton(const GuiRenderMessage& message, const GuiId& guiId, const std::string& caption, const std::vector<GuiLayoutHint>& hints)
{
    GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        glm::vec2 textSize = renderable->MeasureText(message.GetState().Skin.Font, caption, 16.f);
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
                state.Active.Item = {0,0,0};
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
        
        renderable->RenderText(layout->Position + glm::vec2(2,0), message.GetState().Skin.Font, caption, 16.f, glm::vec4(0,0,0,1));
    }
    
    return false;
}

bool GuiControls::DoToggleButton(const GuiRenderMessage& message, const GuiId& guiId, const std::string& caption, const std::vector<GuiLayoutHint>& hints)
{
    GuiData& data = message.GetGuiSystem()->GetData(guiId);
    
    if (!data.Initialised)
    {
        data.Initialised = true;
        data.Data[0].Bool = false;
    }
    
    if (DoButton(message, PbNestedGuiId(guiId, 0), caption, hints))
    {
        data.Data[0].Bool = !data.Data[0].Bool;
    }
   
    return data.Data[0].Bool;
}

std::pair<bool, bool> GuiControls::DoCheckBox(const GuiRenderMessage& message, const GuiId& guiId, bool value, const std::vector<GuiLayoutHint>& hints)
{
    GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->AddLayout("checkbox", guiId, hints, glm::vec2(20.f, 20.f));
        return std::make_pair(false, false);
    }
    
    GuiState& state = message.GetState();
    GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
    
    if (!layout)
    {
        return std::make_pair(false, false);
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeInput)
    {
        bool pressed = false;
        
        glm::vec2 mousePosition = state.MousePosition;
        
        if (mousePosition.x > layout->Position.x &&
            mousePosition.x < layout->Position.x + 20 &&
            mousePosition.y > layout->Position.y &&
            mousePosition.y < layout->Position.y + 20)
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
                state.Active.Item = {0,0,0};
            } else {
                state.Active.Active = true;
            }
        }
        
        return std::make_pair(pressed, !value);
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
        
        renderable->RenderBoxFilled(layout->Position, glm::vec2(20,20), color);
        
        renderable->RenderBoxOutline(layout->Position, glm::vec2(20,20), glm::vec4(0.2,0.2,0.2,1));
        
        if (value)
        {
            renderable->RenderBoxFilled(layout->Position + glm::vec2(2,2), glm::vec2(16,15), glm::vec4(0.2,0.2,0.2,1));
        }
    }
    
    return std::make_pair(false, false);
}

std::pair<bool, std::string> GuiControls::DoTextBox(const GuiRenderMessage& message, const GuiId& guiId, const std::string& value, const std::vector<GuiLayoutHint>& hints)
{
    GuiRenderable* renderable = message.GetGuiComponent()->GetRenderable();
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeLayout)
    {
        glm::vec2 textSize = renderable->MeasureText(message.GetState().Skin.Font, value, 16.f);
        message.GetGuiSystem()->AddLayout("textbox: " + value, guiId, hints, glm::vec2(textSize.x, 20.f));
        return std::pair<bool, std::string>(false, "");
    }
    
    GuiState& state = message.GetState();
    GuiLayout* layout = message.GetGuiSystem()->GetLayout(guiId);
    
    if (!layout)
    {
        return std::pair<bool, std::string>(false, "");
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeInput)
    {
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
            
            if (state.MousePressed)
            {
                state.Keyboard.Item = guiId;
            }
        }
        
        if (state.Keyboard.Item == guiId)
        {
            state.Keyboard.Active = true;
            
            if (state.KeyboardEvent.Type == KeyboardEvent::kKeyboardEventDown)
            {
                if (state.KeyboardEvent.Key == kKeyboardKeyCharacter)
                {
                    return std::pair<bool, std::string>(true, value + state.KeyboardEvent.Character);
                } else if (state.KeyboardEvent.Key == kKeyboardKeyBackspace)
                {
                    return std::pair<bool, std::string>(true, value.substr(0, value.length()-1));
                } else if (state.KeyboardEvent.Key == kKeyboardKeySpace)
                {
                    return std::pair<bool, std::string>(true, value + " ");
                }
            }
        }
    }
    
    if (message.GetEventType() == GuiRenderMessage::kEventTypeRender)
    {
        glm::vec4 color;
        
        if (state.Keyboard.Item == guiId)
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
        
        renderable->RenderText(layout->Position + glm::vec2(2,0), message.GetState().Skin.Font, value, 16.f, glm::vec4(0,0,0,1));
    }
    
    return std::make_pair(false, "");
}
