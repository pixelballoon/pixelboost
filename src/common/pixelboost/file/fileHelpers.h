#pragma once

#include <string>

#include "pixelboost/file/fileSystem.h"

namespace pb
{

namespace FileHelpers
{
    
std::string GetRootPath();
std::string GetUserPath();
bool CreateDirectory(const std::string& directory);
std::string FileToString(FileLocation location, const std::string& filename);
int GetTimestamp(const std::string& filename);
void StringToFile(FileLocation location, const std::string& filename, const std::string& string);
    
}
    
}
