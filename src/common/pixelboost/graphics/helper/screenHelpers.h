#pragma once

#include "glm/glm.hpp"

namespace pb
{

class ScreenHelpers
{
public:
    static bool IsLandscape();
    static bool IsFastDevice();
    static bool IsHighResolution();
    static float GetAspectRatio();
    static glm::vec2 GetScreenResolution();
    static glm::vec2 GetScreenUnits();
    static glm::vec2 GetWorldScale();
    static float GetDpu();
};
    
}
