#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/file/fileSystem.h"

namespace pb
{
namespace FileHelpers
{
    
std::string FileToString(pb::FileLocation location, const std::string& filename)
{
    File* file = pb::FileSystem::Instance()->OpenFile(location, filename);
    
    if (!file)
        return "";
    
    std::string data;
    file->ReadAll(data);
    
    delete file;
    
    return data;
}
    
void StringToFile(const std::string& filename, const std::string& string)
{
    FILE* file = fopen(filename.c_str(), "wb");
    fwrite(string.c_str(), string.length(), 1, file);
    fclose(file);
}

}
    
}
