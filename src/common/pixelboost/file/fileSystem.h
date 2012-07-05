#pragma once

#include <string>
#include <vector>

namespace pb
{
    class File
    {
    public:
        virtual ~File();
        
        virtual bool ReadAll(std::vector<unsigned char>& data) = 0;
        virtual bool ReadAll(std::string& data) = 0;
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
    
    class FileSystem
    {
    public:
        FileSystem();
        ~FileSystem();
        
        static FileSystem* Instance();
        
        File* OpenFile(FileLocation location, const std::string& path);
        bool QueryFile(FileInfo& info, FileLocation location, const std::string& path);
        
        bool CreateDirectory(FileLocation location, const std::string& path);
                
        static FileSystem* _Instance;
    };
    
}
