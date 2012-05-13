#ifndef PIXELBOOST_DISABLE_GRAPHICS

#ifdef PIXELBOOST_PLATFORM_OSX

#include "pixelboost/graphics/helper/screenHelpers.h"
#include "pixelboost/logic/game.h"

#include <sys/sysctl.h>
#include <string>

namespace pb
{
    
namespace ScreenHelpers
{
    
bool IsLandscape()
{
    return Game::Instance()->IsLandscape();
}
    
bool IsFastDevice()
{
    return true;
}

bool IsHighResolution()
{
    return true;
}
    
float GetAspectRatio()
{
    return GetScreenResolution()[1]/GetScreenResolution()[0];
}
    
Vec2 GetScreenResolution()
{
    return Vec2(1024.f, 768.f);
}
    
Vec2 GetWorldScale()
{
    float worldScale = 1.f/((GetScreenResolution()[1]/2)/GetDpu());
    
    return Vec2(worldScale, worldScale);
}
    
float GetDpu()
{
    if (IsHighResolution())
        return 32.f;
    else
        return 16.f;
}
    
}
    
}

#endif

#endif
