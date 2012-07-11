#ifdef PIXELBOOST_PLATFORM_ANDROID

#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/misc/jni.h"

using namespace pb;

class AndroidFile : public File
{
public:
    AndroidFile(FILE* file, long length);

    virtual bool ReadAll(std::vector<unsigned char>& data);
    virtual bool ReadAll(std::string& data);
    
private:
    FILE* _File;
    long _Length;
};

AndroidFile::AndroidFile(FILE* file, long length)
{
    _File = file;
    _Length = length;
}

bool AndroidFile::ReadAll(std::vector<unsigned char>& data)
{
    if (!_File)
        return false;
    
    unsigned char* temp;
    temp = new unsigned char[_Length+1];
    fread(temp, _Length, 1, _File);
    fclose(_File);
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
    fclose(_File);
    temp[_Length] = 0;
    
    data.assign(temp, _Length);
    
    delete[] temp;

    return true;
}

File* FileSystem::OpenFile(FileLocation location, const std::string& path)
{
    std::string actualPath = path.substr(1);

    JNIEnv* env = pb::Jni::GetJniEnv();

    jstring jstr = env->NewStringUTF(actualPath.c_str());
    jclass classId = env->FindClass("com/pixelballoon/pixelboost/PixelboostHelpers");

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
    jlong offset = env->GetLongField(result, offsetField);
    jlong length = env->GetLongField(result, lengthField);

    PbLogDebug("pixelboost", "File opened (%s): %lld (%lld)\n", actualPath.c_str(), offset, length);

    jfieldID fdClassDescriptorFieldID = env->GetFieldID(fdClass, "descriptor", "I");

    jint fd = env->GetIntField(descriptor, fdClassDescriptorFieldID);

    FILE* handle = fdopen(dup(fd), "rb");
    fseek(handle, offset, SEEK_SET);

    if (handle)
        return new AndroidFile(handle, length);
    
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