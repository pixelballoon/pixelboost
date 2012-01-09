#ifdef PIXELBOOST_PLATFORM_IOS

#include "pixelboost/file/fileHelpers.h"

using namespace pixelboost::FileHelpers;

std::string pixelboost::FileHelpers::GetRootPath()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    std::string rootPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
    [pool release];
    return rootPath;
}
    
std::string pixelboost::FileHelpers::GetUserPath()
{
    NSArray *dirPaths;
    dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    
    return std::string([[dirPaths objectAtIndex:0] UTF8String]);
}

#endif
