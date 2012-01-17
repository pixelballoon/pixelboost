#pragma once

#include "pixelboost/math/maths.h"

namespace pixelboost
{

namespace ScreenHelpers
{

bool IsLandscape();
bool IsFastDevice();
bool IsHighResolution();
float GetAspectRatio();
Vec2 GetScreenResolution();
Vec2 GetWorldScale();
float GetDpu();

}
    
}