#ifdef PIXELBOOST_PLATFORM_OSX

#include <Foundation/Foundation.h>

#include "pixelboost/file/fileHelpers.h"

namespace pb
{
    
namespace FileHelpers
{
        
std::string GetBundlePath()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    std::string rootPath = [[[NSBundle mainBundle] resourcePath] UTF8String];
    [pool release];
    return rootPath;
}
    
std::string GetSavePath()
{
    NSArray* dirPaths;
    dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    
    return std::string([[dirPaths objectAtIndex:0] UTF8String]);
}
        
}
    
}

#endif