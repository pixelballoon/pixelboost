#ifndef PIXELBOOST_DISABLE_GRAPHICS

#ifdef PIXELBOOST_PLATFORM_ANDROID

#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"

#include <string>

using namespace pb;

bool ScreenHelpers::IsFastDevice()
{
    return true;
}

#endif

#endif
