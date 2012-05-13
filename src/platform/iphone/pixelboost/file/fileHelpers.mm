#ifdef PIXELBOOST_PLATFORM_IOS

#include "pixelboost/file/fileHelpers.h"

using namespace pb::FileHelpers;

std::string pb::FileHelpers::GetRootPath()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    std::string rootPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
    [pool release];
    return rootPath;
}
    
std::string pb::FileHelpers::GetUserPath()
{
    NSArray *dirPaths;
    dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    
    return std::string([[dirPaths objectAtIndex:0] UTF8String]);
}

#endif
