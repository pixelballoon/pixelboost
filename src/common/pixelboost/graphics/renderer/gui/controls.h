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
        static void BeginArea(const GuiRenderMessage& message, GuiId guiId, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static void EndArea();
        
        static void BeginScrollArea(const GuiRenderMessage& message, GuiId guiId, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static void EndScrollArea();
        
    public:
        static std::string DoCombo(const GuiRenderMessage& message, GuiId guiId, const std::string& label, const std::vector<std::string>& options, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
        static bool DoButton(const GuiRenderMessage& message, GuiId guiId, const std::string& caption, const std::vector<GuiLayoutHint>& hints = std::vector<GuiLayoutHint>());
    };
    
}