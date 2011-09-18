#include "libpixel/logic/game.h"
#include "libpixel/render/screenHelpers.h"

#include <sys/sysctl.h>
#include <string>

namespace libpixel
{
    
namespace ScreenHelpers
{
    
bool UseNativeAspect()
{
    return false;
}
    
bool IsLandscape()
{
    return Game::Instance()->IsLandscape();
}
    
bool DoesNeedBorder()
{
    if ((UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad))
        return true;
    else
        return false;
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
    
bool UseHdGraphics()
{
    return Game::Instance()->UseHdGraphics();
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
    if (UseNativeAspect() && UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
        return 1024.f/768.f;
    else
        return 480.f/320.f;
}
    
float NativeScale()
{
    if ((UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad))
        return 2.f;
    else
        return 1.f;
}
    
Vec2 GetScreenResolution()
{
    if ((UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad))
        return Vec2(768.f, 1024.f);
    else
        return Vec2(320.f, 480.f);
}
    
Vec2 GetWorldScale()
{
    float dpu = 15.f;
    
    float worldScale = 1.f/dpu;
    
    if (!UseNativeAspect() && (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad))
        return Vec2(worldScale * 0.833333f, worldScale * 0.9375f);
    
    return Vec2(worldScale, worldScale);
}
    
}
    
}
