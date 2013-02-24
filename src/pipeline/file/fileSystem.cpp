#include "pipeline/file/fileSystem.h"

using namespace pl;

File::File(FILE* file)
    : _File(file)
{
    
}

File::~File()
{
    fclose(_File);
}

bool File::ReadAll(std::vector<unsigned char>& data)
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
    fseek(_File, 0, SEEK_END);
    len = ftell(_File);
    fseek(_File, 0, SEEK_SET);
    temp = new char[len+1];
    fread(temp, len, 1, _File);
    temp[len] = 0;
    
    data.assign(temp, len);
    
    delete[] temp;
    
    return true;
}

bool File::Read(unsigned char* data, int length)
{
    fread(data, 1, length, _File);
    return true;
}

bool File::Read(char& data)
{
    fread(&data, sizeof(char), 1, _File);
    return true;
}

bool File::Read(float& data)
{
    fread(&data, sizeof(float), 1, _File);
    return true;
}

bool File::Read(short& data)
{
    fread(&data, sizeof(short), 1, _File);
    return true;
}

bool File::Read(int& data)
{
    fread(&data, sizeof(int), 1, _File);
    return true;
}

bool File::Read(bool& data)
{
    fread(&data, sizeof(bool), 1, _File);
    return true;
}

bool File::Write(const std::vector<unsigned char>& data)
{
    fwrite(&data[0], 1, data.size(), _File);
    return true;
}

bool File::Write(const unsigned char* data, int length)
{
    fwrite(data, 1, length, _File);
    return true;
}

bool File::Write(const std::string& data)
{
    fwrite(data.c_str(), 1, data.length(), _File);
    return true;
}

bool File::Write(const char& data)
{
    fwrite(&data, sizeof(char), 1, _File);
    return true;
}

bool File::Write(const float& data)
{
    fwrite(&data, sizeof(float), 1, _File);
    return true;
}

bool File::Write(const short& data)
{
    fwrite(&data, sizeof(short), 1, _File);
    return true;
}

bool File::Write(const int& data)
{
    fwrite(&data, sizeof(int), 1, _File);
    return true;
}

bool File::Write(const bool& data)
{
    fwrite(&data, sizeof(bool), 1, _File);
    return true;
}

bool File::Seek(SeekMode mode, int offset)
{
    int seekMode;
    
    switch (mode)
    {
        case kFileSeekStart:
            seekMode = SEEK_SET;
            break;
        case kFileSeekCurrent:
            seekMode = SEEK_CUR;
            break;
        case kFileSeekEnd:
            seekMode = SEEK_END;
            break;
    }
    
    return fseek(_File, offset, seekMode) == 0;
}

FileSystem::FileSystem()
{
    
}

FileSystem::~FileSystem()
{
    
}

FileSystem* FileSystem::Instance()
{
    static FileSystem* instance = new FileSystem();
    return instance;
}

File* FileSystem::OpenFile(const std::string& filename, FileMode mode)
{
    FILE* file = 0;
    
    file = fopen(filename.c_str(), mode == kFileModeRead ? "rb" : "wb");

    if (!file)
        return 0;
    
    return new File(file);
}
