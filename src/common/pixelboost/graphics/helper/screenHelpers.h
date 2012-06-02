#pragma once

#include "glm/glm.hpp"

namespace pb
{

namespace ScreenHelpers
{

bool IsLandscape();
bool IsFastDevice();
bool IsHighResolution();
float GetAspectRatio();
glm::vec2 GetScreenResolution();
glm::vec2 GetScreenUnits();
glm::vec2 GetWorldScale();
float GetDpu();

}
    
}
