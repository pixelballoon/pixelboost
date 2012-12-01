#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"

#include <string>

using namespace pb;

float ScreenHelpers::GetAspectRatio()
{
    return GraphicsDevice::Instance()->GetDisplayResolution()[1]/GraphicsDevice::Instance()->GetDisplayResolution()[0];
}
    
glm::vec2 ScreenHelpers::GetScreenUnits()
{
    return GraphicsDevice::Instance()->GetDisplayResolution()/GraphicsDevice::Instance()->GetDisplayDensity();
}
    
glm::vec2 ScreenHelpers::GetWorldScale()
{
    float worldScale = 1.f/((GraphicsDevice::Instance()->GetDisplayResolution().y/2)/GraphicsDevice::Instance()->GetDisplayDensity());
    
    return glm::vec2(worldScale, worldScale);
}

#endif
