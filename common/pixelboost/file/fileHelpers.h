#pragma once

#include <string>

namespace pixelboost
{

namespace FileHelpers
{
    
std::string GetRootPath();
std::string FileToString(const std::string& filename);
void StringToFile(const std::string& filename, const std::string& string);
    
}
    
}
