#ifdef PIXELBOOST_PLATFORM_OSX

#include <Foundation/Foundation.h>

#include <sys/stat.h>

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
    dirPaths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    
    std::string path = std::string([[dirPaths objectAtIndex:0] UTF8String]) + "/" + PIXELBOOST_BUILD_IDENTIFIER + "/";
    
    mkdir(path.c_str(), 0755);
    
    return path;
}
        
}
    
}

#endif
