#pragma once

#include <string>
#include <vector>

struct PHYSFS_File;

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

        File(PHYSFS_File* file);
        ~File();
        
        bool ReadAll(std::vector<unsigned char>& data);
        bool ReadAll(std::string& data);
        
        bool Read(unsigned char* data, int length);
        bool Read(char& data);
        bool Read(float& data);
        bool Read(short& data);
        bool Read(int& data);
        bool Read(bool& data);

        bool Write(const std::vector<unsigned char>& data);
        bool Write(const unsigned char* data, int length);
        bool Write(const std::string& data);
        bool Write(const char& data);
        bool Write(const float& data);
        bool Write(const short& data);
        bool Write(const int& data);
        bool Write(const bool& data);

        bool Seek(SeekMode mode, int offset);

    private:
        PHYSFS_File* _File;
    };
    
    class FileInfo
    {
    public:
        int Length;
        int TimeStamp;
    };
    
    enum FileMode
    {
        kFileModeRead,
        kFileModeWrite,
        kFileModeAppend,
    };
    
    class FileSystem
    {
    public:
        FileSystem(const char* appPath);
        ~FileSystem();
        
        static FileSystem* Instance();
        
    public:
        void OverrideWriteDirectory(const std::string& path);
        void MountReadLocation(const std::string& path, const std::string& mountPoint, bool prepend);
        
    public:
        File* OpenFile(const std::string& path, FileMode mode = kFileModeRead);
        bool QueryFile(FileInfo& info, const std::string& path);
        
        bool CreateDirectory(const std::string& path);
        
    private:
        static FileSystem* _Instance;
    };
    
}
