#ifdef PIXELBOOST_PLATFORM_NACL

#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"

using namespace pb;

class NaclFile : public File
{
public:
    NaclFile();
    ~NaclFile();

    virtual bool ReadAll(std::vector<unsigned char>& data);
    virtual bool ReadAll(std::string& data);
    virtual bool Read(float& data);
    virtual bool Read(short& data);
    virtual bool Read(bool& data);

    virtual bool Write(const std::vector<unsigned char>& data);
    virtual bool Write(const std::string& data);
    virtual bool Write(const float& data);
    virtual bool Write(const short& data);
    virtual bool Write(const bool& data);

    virtual bool Seek(SeekMode mode, int offset);
    
private:
};

NaclFile::NaclFile()
{
    
}

NaclFile::~NaclFile()
{
    
}

bool NaclFile::ReadAll(std::vector<unsigned char>& data)
{
    return false;
}

bool NaclFile::ReadAll(std::string& data)
{
    return false;
}


bool NaclFile::Read(float& data)
{
    return false;
}

bool NaclFile::Read(short& data)
{
    return false;
}

bool NaclFile::Read(bool& data)
{
    return false;
}

bool NaclFile::Write(const std::vector<unsigned char>& data)
{
    return false;
}

bool NaclFile::Write(const std::string& data)
{
    return false;
}


bool NaclFile::Write(const float& data)
{
    return false;
}

bool NaclFile::Write(const short& data)
{
    return false;
}

bool NaclFile::Write(const bool& data)
{
    return false;
}

bool NaclFile::Seek(SeekMode mode, int offset)
{
    return false;
}

File* FileSystem::OpenFile(FileLocation location, const std::string& path, FileMode mode)
{
    return 0;
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
