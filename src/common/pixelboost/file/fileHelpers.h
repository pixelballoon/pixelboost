#pragma once

#include <string>

namespace pb
{

namespace FileHelpers
{
    
std::string GetRootPath();
std::string GetUserPath();
bool CreateDirectory(const std::string& directory);
std::string FileToString(const std::string& filename);
int GetTimestamp(const std::string& filename);
void StringToFile(const std::string& filename, const std::string& string);
    
}
    
}
