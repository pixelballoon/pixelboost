#include "libpixel/file/fileHelpers.h"

namespace libpixel
{
    
namespace FileHelpers
{
        
std::string GetRootPath()
{
    return [[[NSBundle mainBundle] bundlePath] UTF8String];
}
        
}
    
}
