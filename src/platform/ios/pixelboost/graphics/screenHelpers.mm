#ifndef PIXELBOOST_DISABLE_GRAPHICS

#ifdef PIXELBOOST_PLATFORM_IOS

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/helper/screenHelpers.h"
#include "pixelboost/logic/game.h"

#include <sys/sysctl.h>
#include <string>

using namespace pb;

bool ScreenHelpers::IsFastDevice()
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

#endif

#endif
