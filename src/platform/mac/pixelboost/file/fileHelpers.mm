#ifdef PIXELBOOST_PLATFORM_OSX

#include "pixelboost/file/fileHelpers.h"

namespace pixelboost
{
    
namespace FileHelpers
{
        
std::string GetRootPath()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    std::string rootPath = [[[NSBundle mainBundle] resourcePath] UTF8String];
    [pool release];
    return rootPath;
}
        
}
    
}

#endif