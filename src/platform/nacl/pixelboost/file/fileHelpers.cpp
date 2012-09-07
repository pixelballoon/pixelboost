#ifdef PIXELBOOST_PLATFORM_NACL

#include "pixelboost/file/fileHelpers.h"

namespace pb
{
    
namespace FileHelpers
{
        
std::string GetRootPath()
{
    return "./";
}
    
std::string GetUserPath()
{
    return "./";
}
    
bool CreateDirectory(const std::string& directory)
{
    return false;
}

int GetTimestamp(const std::string& filename)
{
    return 0;
}
        
}
    
}

#endif