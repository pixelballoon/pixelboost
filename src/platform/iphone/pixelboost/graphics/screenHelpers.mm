#ifdef PIXELBOOST_PLATFORM_IOS

#include "pixelboost/graphics/device/device.h"
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
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0); 
    char* name = (char*)malloc(size);
    sysctlbyname("hw.machine", name, &size, NULL, 0);
    std::string machine = name;
    free(name);
    
    if (machine == "iPhone1,1")  
        return false; // iPhone
    else if (machine == "iPhone1,2")  
        return false; // iPhone 3G
    else if (machine == "iPhone2,1")  
        return true; // iPhone 3GS
    else if (machine == "iPhone3,1")  
        return true; // iPhone 4
    else if (machine == "iPad1,1")  
        return true; // iPad
    else if (machine == "iPod1,1")  
        return false; // iPod
    else if (machine == "iPod2,1")  
        return false; // iPod 2G
    else if (machine == "iPod3,1")  
        return true; // iPod 3G
    else if (machine == "iPod4,1")  
        return true; // iPod 4G
    
    return true;
}

bool IsHighResolution()
{
    if (([[UIScreen mainScreen] respondsToSelector:@selector(scale)] == YES && [[UIScreen mainScreen] scale] == 2.00) ||
        (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad))
        return true;
    
    return false;
}
    
float GetAspectRatio()
{
    return GetScreenResolution()[1]/GetScreenResolution()[0];
}
    
glm::vec2 GetScreenResolution()
{
    return pb::GraphicsDevice::Instance()->GetDisplayResolution();
}
    
glm::vec2 GetScreenUnits()
{
    return GetScreenResolution()/GetDpu();
}
    
glm::vec2 GetWorldScale()
{
    float worldScale = 1.f/((GetScreenResolution().y/2)/GetDpu());
    
    return glm::vec2(worldScale, worldScale);
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
