#ifndef LIBPIXEL__RENDER__SCREENHELPERS__H
#define LIBPIXEL__RENDER__SCREENHELPERS__H

#include "libpixel/math/maths.h"

namespace libpixel
{

namespace ScreenHelpers
{

bool IsLandscape();
bool IsFastDevice();
bool UseHdGraphics();
bool IsHighResolution();
float GetAspectRatio();
Vec2 GetScreenResolution();
Vec2 GetWorldScale();
float GetDpu();

}
    
}

#endif
