#ifdef PIXELBOOST_PLATFORM_ANDROID

#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/misc/jni.h"

using namespace pb;

class AndroidFile : public File
{
public:
    AndroidFile(FILE* file, long length, long startOffset);
    ~AndroidFile();

    virtual bool ReadAll(std::vector<unsigned char>& data);
    virtual bool ReadAll(std::string& data);

    virtual bool Write(const std::vector<unsigned char>& data);
    virtual bool Write(const std::string& data);

    virtual bool Seek(SeekMode mode, int offset);
    
private:
    FILE* _File;
    long _Length;
    long _StartOffset;
};

AndroidFile::AndroidFile(FILE* file, long length, long startOffset)
    : _File(file)
    , _Length(length)
    , _StartOffset(startOffset)
{
    
}

AndroidFile::~AndroidFile()
{
    fclose(_File);
    _File = 0;
}

bool AndroidFile::ReadAll(std::vector<unsigned char>& data)
{
    if (!_File)
        return false;
    
    unsigned char* temp;
    temp = new unsigned char[_Length+1];
    fread(temp, _Length, 1, _File);
    temp[_Length] = 0;
    
    data.assign(temp, temp+_Length);
    
    delete[] temp;
    
    return true;
}

bool AndroidFile::ReadAll(std::string& data)
{
    if (!_File)
        return false;
    
    char* temp;
    temp = new char[_Length+1];
    fread(temp, _Length, 1, _File);
    temp[_Length] = 0;
    
    data.assign(temp, _Length);
    
    delete[] temp;

    return true;
}

bool AndroidFile::Write(const std::vector<unsigned char>& data)
{
    if (!_File)
        return false;
    
    fwrite(&data[0], 1, data.size(), _File);
    
    return true;
}

bool AndroidFile::Write(const std::string& data)
{
    if (!_File)
        return false;
    
    long written = fwrite(data.c_str(), 1, data.length(), _File);

    return true;
}

bool AndroidFile::Seek(SeekMode mode, int offset)
{
    int fseekMode;

    switch (mode)
    {
        case kFileSeekStart:
            fseekMode = SEEK_SET;
            offset = _StartOffset + offset;
            break;
        case kFileSeekCurrent:
            fseekMode = SEEK_CUR;
            break;
        case kFileSeekEnd:
            fseekMode = SEEK_SET;
            offset = _StartOffset + _Length + offset;
            break;
    }

    return fseek(_File, offset, fseekMode) == 0;
}

File* FileSystem::OpenFile(FileLocation location, const std::string& path, FileMode mode)
{
    std::string actualPath = path.substr(1);

    JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(actualPath.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");

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

    FILE* handle = 0;
    jlong offset = 0;
    jlong length = 0;

    if (location == kFileLocationBundle)
    {
        jmethodID methodId = env->GetStaticMethodID(classId, "openFileDescriptor", "(Ljava/lang/String;)Lcom/pixelballoon/pixelboost/FileInfo;");
        jobject result = env->CallStaticObjectMethod(classId, methodId, jstr);

        env->DeleteLocalRef(jstr);

        if (!result)
            return 0;

        jclass fiClass = env->FindClass("com/pixelballoon/pixelboost/FileInfo");

        if (!fiClass)
            return 0;

        jclass fdClass = env->FindClass("java/io/FileDescriptor");

        if (!fdClass)
            return 0;

        jfieldID descriptorField = env->GetFieldID(fiClass, "descriptor", "Ljava/io/FileDescriptor;");
        jfieldID offsetField = env->GetFieldID(fiClass, "offset", "J");
        jfieldID lengthField = env->GetFieldID(fiClass, "length", "J");

        jobject descriptor = env->GetObjectField(result, descriptorField);
        offset = env->GetLongField(result, offsetField);
        length = env->GetLongField(result, lengthField);

        PbLogDebug("pixelboost", "Opening bundle file (%s): %lld (%lld)\n", actualPath.c_str(), offset, length);

        jfieldID fdClassDescriptorFieldID = env->GetFieldID(fdClass, "descriptor", "I");

        jint fd = env->GetIntField(descriptor, fdClassDescriptorFieldID);

        handle = fdopen(dup(fd), openMode.c_str());
        fseek(handle, offset, SEEK_SET);
    } else if (location == kFileLocationUser) {
        jmethodID methodId = env->GetStaticMethodID(classId, "getUserFilePath", "()Ljava/lang/String;");
        jstring result = static_cast<jstring>(env->CallStaticObjectMethod(classId, methodId));

        const char *nativeString = env->GetStringUTFChars(result, 0);
        std::string totalPath = nativeString;
        env->ReleaseStringUTFChars(result, nativeString);

        totalPath += path;

        PbLogDebug("pixelboost", "Opening user file (%s)", totalPath.c_str());

        handle = fopen(totalPath.c_str(), openMode.c_str());

        if (handle)
        {
            fseek(handle, 0, SEEK_END);
            length = ftell(handle);
            fseek(handle, 0, SEEK_SET);
        }
    }

    if (handle)
        return new AndroidFile(handle, length, offset);

    PbLogDebug("pixelboost", "Opening file (%s) failed", path.c_str());
    
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
