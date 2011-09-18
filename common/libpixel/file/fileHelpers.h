#ifndef LIBPIXEL__FILE__HELPERS__H
#define LIBPIXEL__FILE__HELPERS__H

#include <string>

namespace libpixel
{

namespace FileHelpers
{
    
std::string GetRootPath();
std::string FileToString(const std::string& filename);
void StringToFile(const std::string& filename, const std::string& string);
    
}
    
}

#endif