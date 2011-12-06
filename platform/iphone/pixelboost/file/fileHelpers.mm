#include "pixelboost/file/fileHelpers.h"

namespace pixelboost
{
    
namespace FileHelpers
{
        
std::string GetRootPath()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    std::string rootPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
    [pool release];
    return rootPath;
}
        
}
    
}
