#include "pixelboost/debug/assert.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/file/fileSystem.h"

#ifdef PIXELBOOST_PLATFORM_IOS
#define PIXELBOOST_STANDARD_FILESYSTEM
#endif

#ifdef PIXELBOOST_PLATFORM_OSX
#define PIXELBOOST_STANDARD_FILESYSTEM
#endif

#ifdef PIXELBOOST_PLATFORM_WINDOWS
#define PIXELBOOST_STANDARD_FILESYSTEM
#endif

using namespace pb;

FileSystem* FileSystem::_Instance = 0;

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
    ~StdFile();

    virtual bool ReadAll(std::vector<unsigned char>& data);
    virtual bool ReadAll(std::string& data);
    
    virtual bool Read(unsigned char* data, int length);
    virtual bool Read(char& data);
    virtual bool Read(float& data);
    virtual bool Read(short& data);
    virtual bool Read(int& data);
    virtual bool Read(bool& data);
    
    virtual bool Write(const std::vector<unsigned char>& data);
    virtual bool Write(const unsigned char* data, int length);
    virtual bool Write(const std::string& data);
    virtual bool Write(const char& data);
    virtual bool Write(const float& data);
    virtual bool Write(const short& data);
    virtual bool Write(const int& data);
    virtual bool Write(const bool& data);
    
    virtual bool Seek(SeekMode mode, int offset);
    
private:
    FILE* _File;
};

StdFile::StdFile(FILE* file)
{
    _File = file;
}

StdFile::~StdFile()
{
    fclose(_File);
    _File = 0;
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

bool StdFile::Read(unsigned char* data, int length)
{
    fread(data, 1, length, _File);
    return true;
}

bool StdFile::Read(char& data)
{
    fread(&data, sizeof(char), 1, _File);
    return true;
}

bool StdFile::Read(float& data)
{
    fread(&data, sizeof(float), 1, _File);
    return true;
}

bool StdFile::Read(short& data)
{
    fread(&data, sizeof(short), 1, _File);
    return true;
}

bool StdFile::Read(int& data)
{
    fread(&data, sizeof(int), 1, _File);
    return true;
}

bool StdFile::Read(bool& data)
{
    fread(&data, sizeof(bool), 1, _File);
    return true;
}

bool StdFile::Write(const std::vector<unsigned char>& data)
{
    if (!_File)
        return false;
    
    fwrite(&data[0], 1, data.size(), _File);
    
    return true;
}

bool StdFile::Write(const unsigned char* data, int length)
{
    if (!_File)
        return false;
    
    fwrite(data, 1, length, _File);
    
    return true;
}

bool StdFile::Write(const std::string& data)
{
    if (!_File)
        return false;
    
    fwrite(data.c_str(), 1, data.length(), _File);
    
    return true;
}

bool StdFile::Write(const char& data)
{
    if (!_File)
        return false;
    
    fwrite(&data, sizeof(char), 1, _File);
    
    return true;
}

bool StdFile::Write(const float& data)
{
    if (!_File)
        return false;
    
    fwrite(&data, sizeof(float), 1, _File);
    
    return true;
}

bool StdFile::Write(const short& data)
{
    if (!_File)
        return false;
    
    fwrite(&data, sizeof(short), 1, _File);
    
    return true;
}

bool StdFile::Write(const int& data)
{
    if (!_File)
        return false;
    
    fwrite(&data, sizeof(int), 1, _File);
    
    return true;
}

bool StdFile::Write(const bool& data)
{
    if (!_File)
        return false;
    
    fwrite(&data, sizeof(bool), 1, _File);
    
    return true;
}

bool StdFile::Seek(SeekMode mode, int offset)
{
    int fseekMode;

    switch (mode)
    {
        case kFileSeekStart:
            fseekMode = SEEK_SET;
            break;
        case kFileSeekCurrent:
            fseekMode = SEEK_CUR;
            break;
        case kFileSeekEnd:
            fseekMode = SEEK_END;
            break;
    }

    return fseek(_File, offset, fseekMode) == 0;
}

File* FileSystem::OpenFile(FileLocation location, const std::string& path, FileMode mode)
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
    
    std::string openMode;
    
    switch (mode)
    {
        case kFileModeReadOnly:
            openMode = "rb";
            break;
        case kFileModeReadWrite:
            openMode = "r+b";
            break;
        case kFileModeWriteOnly:
            openMode = "wb";
            break;
    }
    
    absolutePath += path;
    FILE* handle = fopen(absolutePath.c_str(), openMode.c_str());
    
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
