#pragma once

#include <cstdio>
#include <string>
#include <vector>

namespace pl
{
    
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

    class File
    {
    protected:
        File(FILE* file);
        
    public:
        enum SeekMode
        {
            kFileSeekStart,
            kFileSeekCurrent,
            kFileSeekEnd,
        };
        
        virtual ~File();
        
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
        FILE* _File;
        
        friend class FileSystem;
    };

    class FileSystem
    {
    public:
        FileSystem();
        ~FileSystem();

        static FileSystem* Instance();

        File* OpenFile(const std::string& path, FileMode mode = kFileModeRead);
    };

}
