#ifndef PIXELBOOST_DISABLE_GRAPHICS

#ifdef PIXELBOOST_PLATFORM_OSX

#include "pixelboost/graphics/helper/screenHelpers.h"
#include "pixelboost/logic/game.h"

#include <sys/sysctl.h>
#include <string>

using namespace pb;

bool ScreenHelpers::IsFastDevice()
{
    return true;
}

#endif

#endif
