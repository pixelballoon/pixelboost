#pragma once

#include "glm/glm.hpp"

namespace pb
{

class ScreenHelpers
{
public:
    static float GetAspectRatio();
    static glm::vec2 GetScreenUnits();
    static glm::vec2 GetWorldScale();
};
    
}
