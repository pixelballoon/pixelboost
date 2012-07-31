#pragma once

#include <string>
#include <vector>

namespace pb
{
    class File
    {
    public:
        enum SeekMode
        {
            kFileSeekStart,
            kFileSeekCurrent,
            kFileSeekEnd,
        };

        virtual ~File();
        
        virtual bool ReadAll(std::vector<unsigned char>& data) = 0;
        virtual bool ReadAll(std::string& data) = 0;

        virtual bool Write(const std::vector<unsigned char>& data) = 0;
        virtual bool Write(const std::string& data) = 0;

        virtual bool Seek(SeekMode mode, int offset) = 0;
    };
    
    class FileInfo
    {
    public:
        int Length;
        int TimeStamp;
    };
    
    enum FileLocation
    {
        kFileLocationBundle,
        kFileLocationUser
    };

    enum FileMode
    {
        kFileModeReadOnly,
        kFileModeWriteOnly,
        kFileModeReadWrite,
    };
    
    class FileSystem
    {
    public:
        FileSystem();
        ~FileSystem();
        
        static FileSystem* Instance();
        
        File* OpenFile(FileLocation location, const std::string& path, FileMode mode = kFileModeReadOnly);
        bool QueryFile(FileInfo& info, FileLocation location, const std::string& path);
        
        bool CreateDirectory(FileLocation location, const std::string& path);
                
        static FileSystem* _Instance;
    };
    
}
