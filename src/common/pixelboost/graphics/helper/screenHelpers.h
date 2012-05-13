#pragma once

#include "pixelboost/math/maths.h"

namespace pb
{

namespace ScreenHelpers
{

bool IsLandscape();
bool IsFastDevice();
bool IsHighResolution();
float GetAspectRatio();
Vec2 GetScreenResolution();
Vec2 GetScreenUnits();
Vec2 GetWorldScale();
float GetDpu();

}
    
}
