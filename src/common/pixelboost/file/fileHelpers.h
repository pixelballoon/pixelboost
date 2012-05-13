#pragma once

#include <string>

namespace pb
{

namespace FileHelpers
{
    
std::string GetRootPath();
std::string GetUserPath();
std::string FileToString(const std::string& filename);
void StringToFile(const std::string& filename, const std::string& string);
    
}
    
}
