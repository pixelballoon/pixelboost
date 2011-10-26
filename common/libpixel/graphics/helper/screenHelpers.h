#pragma once

#include "libpixel/math/maths.h"

namespace libpixel
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
