#ifdef PIXELBOOST_PLATFORM_WINDOWS

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
    
bool pb::FileHelpers::CreateDirectory(const std::string& directory)
{
    return false;
}

int pb::FileHelpers::GetTimestamp(const std::string& filename)
{
    return 0;
}
        
}
    
}

#endif