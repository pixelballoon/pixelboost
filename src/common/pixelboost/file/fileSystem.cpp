#include "physfs.h"

#include "pixelboost/debug/assert.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/file/fileSystem.h"

using namespace pb;

FileSystem* FileSystem::_Instance = 0;

File::File(PHYSFS_File* file)
{
    _File = file;
}

File::~File()
{
    PHYSFS_close(_File);
}

bool File::ReadAll(std::vector<unsigned char>& data)
{
    if (!_File)
        return false;
    
    long len;
    unsigned char* temp;
    len = PHYSFS_fileLength(_File);
    PHYSFS_seek(_File, 0);
    temp = new unsigned char[len+1];
    PHYSFS_read(_File, temp, len, 1);
    temp[len] = 0;
    
    data.assign(temp, temp+len);
    
    delete[] temp;
    
    return true;
}

bool File::ReadAll(std::string& data)
{
    if (!_File)
        return false;
    
    long len;
    char* temp;
    len = PHYSFS_fileLength(_File);
    PHYSFS_seek(_File, 0);
    temp = new char[len+1];
    PHYSFS_read(_File, temp, len, 1);
    temp[len] = 0;
    
    data.assign(temp, len);
    
    delete[] temp;
    
    return true;
}

bool File::Read(unsigned char* data, int length)
{
    PHYSFS_read(_File, data, 1, length);
    return true;
}

bool File::Read(char& data)
{
    PHYSFS_read(_File, &data, sizeof(char), 1);
    return true;
}

bool File::Read(float& data)
{
    PHYSFS_read(_File, &data, sizeof(float), 1);
    return true;
}

bool File::Read(short& data)
{
    PHYSFS_read(_File, &data, sizeof(short), 1);
    return true;
}

bool File::Read(int& data)
{
    PHYSFS_read(_File, &data, sizeof(int), 1);
    return true;
}

bool File::Read(bool& data)
{
    PHYSFS_read(_File, &data, sizeof(bool), 1);
    return true;
}

bool File::Write(const std::vector<unsigned char>& data)
{
    PHYSFS_write(_File, &data[0], 1, data.size());
    return true;
}

bool File::Write(const unsigned char* data, int length)
{
    PHYSFS_write(_File, data, 1, length);
    return true;
}

bool File::Write(const std::string& data)
{
    PHYSFS_write(_File, data.c_str(), 1, data.length());
    return true;
}

bool File::Write(const char& data)
{
    PHYSFS_write(_File, &data, sizeof(char), 1);
    return true;
}

bool File::Write(const float& data)
{
    PHYSFS_write(_File, &data, sizeof(float), 1);
    return true;
}

bool File::Write(const short& data)
{
    PHYSFS_write(_File, &data, sizeof(short), 1);
    return true;
}

bool File::Write(const int& data)
{
    PHYSFS_write(_File, &data, sizeof(int), 1);
    return true;
}

bool File::Write(const bool& data)
{
    PHYSFS_write(_File, &data, sizeof(bool), 1);
    return true;
}

bool File::Seek(SeekMode mode, int offset)
{
    int filePosition;
    
    switch (mode)
    {
        case kFileSeekStart:
            filePosition = offset;
            break;
        case kFileSeekCurrent:
            filePosition = PHYSFS_tell(_File) + offset;
            break;
        case kFileSeekEnd:
            filePosition = PHYSFS_fileLength(_File) + offset;
            break;
    }
    
    return PHYSFS_seek(_File, filePosition) == 0;
}

FileSystem::FileSystem(const char* appPath)
{
    _Instance = this;
    
    PHYSFS_init(appPath);
    
    PHYSFS_mount(pb::FileHelpers::GetBundlePath().c_str(), "/", 0);
    PHYSFS_setWriteDir(pb::FileHelpers::GetSavePath().c_str());
}

FileSystem::~FileSystem()
{
    
}

FileSystem* FileSystem::Instance()
{
    return _Instance;
}

void FileSystem::OverrideWriteDirectory(const std::string& path)
{
    PHYSFS_setWriteDir(path.c_str());
}

void FileSystem::MountReadLocation(const std::string& path, const std::string& mountPoint, bool prepend)
{
    PHYSFS_mount(path.c_str(), mountPoint.c_str(), prepend ? 0 : 1);
}

File* FileSystem::OpenFile(const std::string& path, FileMode mode)
{
    PHYSFS_file* handle = 0;
    
    switch (mode)
    {
        case kFileModeRead:
        {
            handle = PHYSFS_openRead(path.c_str());
            // Special case for Android, since the assets are contained within a subfolder within the zip we have
            // to try opening with that prefix. We try without the prefix first to allow for user specified files
            // to override the bundle assets. This isn't perfect, as it effectively disallows assets to be in the
            // bundle, but it will do until PHYSFS is modified to allow a subdirectory within a zip to be mounted
            #ifdef PIXELBOOST_PLATFORM_ANDROID
            if (!handle)
                handle = PHYSFS_openRead(("assets/"+path).c_str());
            #endif
            break;
        }
        case kFileModeWrite:
            handle = PHYSFS_openWrite(path.c_str());
            break;
        case kFileModeAppend:
            handle = PHYSFS_openAppend(path.c_str());
            break;
    }
    
    if (!handle)
        return 0;
    
    return new File(handle);
}

bool FileSystem::QueryFile(FileInfo& info, const std::string& path)
{
    PbAssert(!"Not yet implemented");
    
    return false;
}

bool FileSystem::CreateDirectory(const std::string& path)
{
    return PHYSFS_mkdir(path.c_str());
}
