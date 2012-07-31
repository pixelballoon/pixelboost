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
    
void StringToFile(pb::FileLocation location, const std::string& filename, const std::string& string)
{
    File* file = pb::FileSystem::Instance()->OpenFile(location, filename, pb::kFileModeWriteOnly);
    
    if (!file)
        return;
    
    file->Write(string);
    delete file;
}

}
    
}
