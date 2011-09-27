#ifndef LIBPIXEL__RENDER__SCREENHELPERS__H
#define LIBPIXEL__RENDER__SCREENHELPERS__H

#include "libpixel/math/maths.h"

namespace libpixel
{

namespace ScreenHelpers
{

bool UseNativeAspect();
bool IsLandscape();
bool DoesNeedBorder();
bool IsFastDevice();
bool UseHdGraphics();
bool IsHighResolution();
float GetAspectRatio();
float NativeScale();
Vec2 GetScreenResolution();
Vec2 GetWorldScale();

}
    
}

#endif
