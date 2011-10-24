#ifdef LIBPIXEL_PLATFORM_OSX

#include "libpixel/file/fileHelpers.h"

namespace libpixel
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