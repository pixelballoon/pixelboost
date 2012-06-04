#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"

#include <sys/sysctl.h>
#include <string>

using namespace pb;
    
bool ScreenHelpers::IsLandscape()
{
    return Game::Instance()->IsLandscape();
}

bool ScreenHelpers::IsHighResolution()
{
    if (GetDpu() >= 32.f)
        return true;
        
    return false;
}
    
float ScreenHelpers::GetAspectRatio()
{
    return GetScreenResolution()[1]/GetScreenResolution()[0];
}
    
glm::vec2 ScreenHelpers::GetScreenResolution()
{
    return pb::GraphicsDevice::Instance()->GetDisplayResolution();
}
    
glm::vec2 ScreenHelpers::GetScreenUnits()
{
    return GetScreenResolution()/GetDpu();
}
    
glm::vec2 ScreenHelpers::GetWorldScale()
{
    float worldScale = 1.f/((GetScreenResolution().y/2)/GetDpu());
    
    return glm::vec2(worldScale, worldScale);
}
    
float ScreenHelpers::GetDpu()
{
    return GraphicsDevice::Instance()->GetDisplayDensity();
}

#endif
