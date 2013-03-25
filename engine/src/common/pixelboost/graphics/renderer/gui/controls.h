#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/logic/system/graphics/gui/gui.h"


namespace pb
{
    
    class GuiRenderMessage;
    
    class GuiControls
    {
    public:
        static const GuiId& BeginArea(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static void EndArea(const GuiRenderMessage& message, const GuiId& guiId);
        
        static void BeginHorizontal(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static void EndHorizontal(const GuiRenderMessage& message);

        static void BeginVertical(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static void EndVertical(const GuiRenderMessage& message);
        
        static const GuiId& BeginScrollArea(const GuiRenderMessage& message, const GuiId& guiId, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static void EndScrollArea(const GuiRenderMessage& message, const GuiId& guiId);
        
    public:
        static int DoCombo(const GuiRenderMessage& message, const GuiId& guiId, const std::string& label, const std::vector<std::string>& options, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static void DoLabel(const GuiRenderMessage& message, const GuiId& guiId, const std::string& label, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static bool DoButton(const GuiRenderMessage& message, const GuiId& guiId, const std::string& caption, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static std::pair<bool, std::string> DoTextBox(const GuiRenderMessage& message, const GuiId& guiId, const std::string& value, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
    };
    
}