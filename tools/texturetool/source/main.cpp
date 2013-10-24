#include <iostream>
#include <sstream>
#include <string>

#include "pipeline/debug/log.h"
#include "pipeline/file/fileSystem.h"

#include "jpegcompressor/jpge.h"
#include "lodepng/lodepng.h"
#include "stbimage/stb_image.h"
#include "stbimage/stb_image_write.h"

int main(int argc, const char * argv[])
{
    if (argc < 2)
        return 1;
    
    std::string inputLocation = argv[1];
    std::string outputLocation = argv[2];
    
    std::vector<unsigned char> data;
    unsigned char* decoded;
    
    PlLogInfo("texturetool", "Converting %s to %s", argv[1], argv[2]);
    
    pl::File* file = pl::FileSystem::Instance()->OpenFile(inputLocation, pl::kFileModeRead);
    if (!file)
    {
        PlLogError("texturetool", "Can't open input file");
        return 1;
    }
    
    file->ReadAll(data);
    delete file;
    file = 0;
    
    int width, height, components;
    decoded = stbi_load_from_memory(&data[0], data.size(), &width, &height, &components, STBI_default);

    if (components == 3)
    {
        PlLogError("texturetool", "Only PNGs with alpha are supported");
        return 1;
    }
    
    unsigned char* rgbData = new unsigned char[width*height*3];
    unsigned char* alphaData = new unsigned char[width*height];
    
    unsigned char* decodedTemp = decoded;
    unsigned char* rgbTemp = rgbData;
    unsigned char* alphaTemp = alphaData;
    
    for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            rgbTemp[0] = decodedTemp[0];
            rgbTemp[1] = decodedTemp[1];
            rgbTemp[2] = decodedTemp[2];
            *alphaTemp = decodedTemp[3];
            
            decodedTemp += 4;
            rgbTemp += 3;
            alphaTemp++;
        }
    }
    
    int rgbBufferSize = width*height*3;
    unsigned char* rgbBuffer = new unsigned char[rgbBufferSize];
    
    jpge::params params;
    params.m_quality = 95;
    params.m_two_pass_flag = true;
    
    jpge::compress_image_to_jpeg_file_in_memory(rgbBuffer, rgbBufferSize, width, height, 3, rgbData, params);
    
    int alphaBufferSize;
    unsigned char* alphaBuffer = stbi_write_png_to_mem(alphaData, 0, width, height, 1, &alphaBufferSize);
    
    file = pl::FileSystem::Instance()->OpenFile(outputLocation, pl::kFileModeWrite);
    
    if (!file)
    {
        PlLogError("texturetool", "Can't open output file");
        return 1;
    }
    
    file->Write((int)rgbBufferSize);
    file->Write((int)alphaBufferSize);
    file->Write(rgbBuffer, rgbBufferSize);
    file->Write(alphaBuffer, alphaBufferSize);

    delete file;
    
    delete[] rgbBuffer;
    free(alphaBuffer);
    
    return 0;
}
