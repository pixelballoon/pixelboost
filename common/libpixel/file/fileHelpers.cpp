#include "libpixel/file/fileHelpers.h"

namespace libpixel
{
namespace FileHelpers
{
    
std::string FileToString(const std::string& filename)
{
    FILE* file = fopen(filename.c_str(), "rb");
    
    if (!file)
        return "";
    
    long len;
    char* temp;
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);
    temp = new char[len+1];
    fread(temp, len, 1, file);
    fclose(file);
    temp[len] = 0;
    
    std::string returnValue = temp;
    delete[] temp;

    return returnValue;
}
    
void StringToFile(const std::string& filename, const std::string& string)
{
    FILE* file = fopen(filename.c_str(), "wb");
    fwrite(string.c_str(), string.length(), 1, file);
    fclose(file);
}

}
    
}
