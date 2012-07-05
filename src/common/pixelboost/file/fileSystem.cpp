#include "pixelboost/debug/assert.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/file/fileSystem.h"

#ifdef PIXELBOOST_PLATFORM_IOS
#define PIXELBOOST_STANDARD_FILESYSTEM
#endif

#ifdef PIXELBOOST_PLATFORM_MAC
#define PIXELBOOST_STANDARD_FILESYSTEM
#endif

using namespace pb;

FileSystem* FileSystem::FileSystem::_Instance = 0;

File::~File()
{
    
}

FileSystem::FileSystem()
{
    _Instance = this;
}

FileSystem::~FileSystem()
{
    
}

FileSystem* FileSystem::Instance()
{
    return _Instance;
}

#ifdef PIXELBOOST_STANDARD_FILESYSTEM

class StdFile : public File
{
public:
    StdFile(FILE* file);

    virtual bool ReadAll(std::vector<unsigned char>& data);
    virtual bool ReadAll(std::string& data);
    
private:
    FILE* _File;
};

StdFile::StdFile(FILE* file)
{
    _File = file;
}

bool StdFile::ReadAll(std::vector<unsigned char>& data)
{
    if (!_File)
        return false;
    
    long len;
    unsigned char* temp;
    fseek(_File, 0, SEEK_END);
    len = ftell(_File);
    fseek(_File, 0, SEEK_SET);
    temp = new unsigned char[len+1];
    fread(temp, len, 1, _File);
    fclose(_File);
    temp[len] = 0;
    
    data.assign(temp, temp+len);
    
    delete[] temp;
    
    return true;
}

bool StdFile::ReadAll(std::string& data)
{
    if (!_File)
        return false;
    
    long len;
    char* temp;
    fseek(_File, 0, SEEK_END);
    len = ftell(_File);
    fseek(_File, 0, SEEK_SET);
    temp = new char[len+1];
    fread(temp, len, 1, _File);
    fclose(_File);
    temp[len] = 0;
    
    data.assign(temp, len);
    
    delete[] temp;
    
    return true;
}

File* FileSystem::OpenFile(FileLocation location, const std::string& path)
{
    std::string absolutePath;
    
    switch (location)
    {
        case kFileLocationBundle:
        {
            absolutePath = pb::FileHelpers::GetRootPath();
            break;
        }
        case kFileLocationUser:
        {
            absolutePath = pb::FileHelpers::GetUserPath();
            break;
        }
    }
    
    absolutePath += path;
    FILE* handle = fopen(absolutePath.c_str(), "rb");
    
    if (!handle)
        return 0;
    
    return new StdFile(handle);
}

bool FileSystem::QueryFile(FileInfo& info, FileLocation location, const std::string& path)
{
    PbAssert(!"Not yet implemented");
    
    return false;
}

bool FileSystem::CreateDirectory(FileLocation location, const std::string& path)
{
    PbAssert(!"Not yet implemented");
    
    return false;
}

#endif
