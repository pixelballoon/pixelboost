#include <iostream>
#include <sstream>
#include <string>

#include "jpegcompressor/jpge.h"
#include "lodepng/lodepng.h"
#include "stbimage/stb_image.h"

#include "pixelboost/file/fileHelpers.h"

int main(int argc, const char * argv[])
{
    if (argc < 2)
        return 1;
    
    std::string inputLocation = argv[1];
    std::string outputLocation = argv[2];
    
    std::vector<unsigned char> data;
    unsigned char* decoded;
    
    printf("Converting %s to %s\n", argv[1], argv[2]);
    
    pb::File* file = pb::FileSystem::Instance()->OpenFile(pb::kFileLocationUser, inputLocation);
    if (!file)
        return false;
    
    file->ReadAll(data);
    
    delete file;
    
    int width, height, components;
    decoded = stbi_load_from_memory(&data[0], data.size(), &width, &height, &components, STBI_default);

    if (components == 3)
    {
        printf("ERROR: Only PNGs with alpha are supported\n");
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
            alphaData++;
        }
    }
    
    int rgbBufferSize = width*height*3;
    unsigned char* buffer = new unsigned char[rgbBufferSize];
    
    jpge::params params;
    params.m_quality = 85;
    params.m_two_pass_flag = true;
    
    jpge::compress_image_to_jpeg_file_in_memory(buffer, rgbBufferSize, width, height, 3, rgbData, params);
    
    std::vector<unsigned char> jpegRGBFile;
    jpegRGBFile.assign(buffer, buffer+rgbBufferSize);
        
    delete[] buffer;
    
    size_t alphaBufferSize;
    LodePNG_encode(&buffer, &alphaBufferSize, alphaData, width, height, LCT_GREY, 8);
    
    std::vector<unsigned char> jpegAlphaFile;
    jpegAlphaFile.assign(buffer, buffer+alphaBufferSize);
    
    delete[] buffer;
    
    file = pb::FileSystem::Instance()->OpenFile(pb::kFileLocationUser, outputLocation, pb::kFileModeWriteOnly);
    
    if (!file)
        return 1;
    
    file->Write((int)rgbBufferSize);
    file->Write((int)alphaBufferSize);
    file->Write(jpegRGBFile);
    file->Write(jpegAlphaFile);

    delete file;
    
    printf("Success!\n");
    
    return 0;
}
