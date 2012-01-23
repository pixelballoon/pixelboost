//
//  main.cpp
//  hullgenerator
//
//  Created by Jamie Hales on 23/01/2012.
//  Copyright (c) 2012 Moshen Ltd. All rights reserved.
//

#include <iostream>

#include "pixelboost/math/maths.h"
#include "pixelboost/external/lodepng/lodepng.h"

class HullGenerator
{
public:
    bool Load(const std::string& filename);
    bool Process();
    
private:
    void GenerateFrame(int x, int y);
    
    enum FrameValue
    {
        kFrameValueUnset,
        kFrameValueEmpty,
        kFrameValueBorder,
        kFrameValueImage,
    };
    
    int GetIndex(int x, int y);
    FrameValue GetFrame(int x, int y);
    void SetFrame(int x, int y, FrameValue value);
                 
    std::vector<unsigned char> _Image;
    int _Width;
    int _Height;
    
    std::vector<FrameValue> _Frame;
    std::vector<Vec2> _Border;
};

int main (int argc, const char * argv[])
{
    const char* filename = argc > 1 ? argv[1] : "default.png";
    
    HullGenerator hullGenerator;
    hullGenerator.Load(filename);
    hullGenerator.Process();

    return 0;
}

bool HullGenerator::Load(const std::string &filename)
{
    std::vector<unsigned char> buffer;
    
    LodePNG::loadFile(buffer, filename); //load the image file with given filename
    LodePNG::Decoder decoder;
    
    decoder.decode(_Image, buffer.size() ? &buffer[0] : 0, (unsigned)buffer.size());
    
    if(decoder.hasError())
    {
        std::cout << "error " << decoder.getError() << ": "<< LodePNG_error_text(decoder.getError()) << std::endl;
        return false;
    }
    
    _Width = decoder.getWidth(); //get the width in pixels
    _Height = decoder.getHeight(); //get the height in pixels
    
    return true;
}

bool HullGenerator::Process()
{
    if (!_Image.size())
        return false;
    
    _Frame.clear();
    _Frame.reserve(_Image.size()/4);
    
    for (int i=0; i<_Image.size()/4; i++)
    {
        _Frame[i] = kFrameValueUnset;
    }
    
    GenerateFrame(0, 0);
    
    // Debug

    for (int y=0; y<_Height; y++)
    {
        for (int x=0; x<_Width; x++)
        {
            std::cout << ((GetFrame(x, y) == kFrameValueBorder) ? "X" : " ");
        }
        
        std::cout << std::endl;
    }
    
    return true;
}

void HullGenerator::GenerateFrame(int x, int y)
{
    if (x < 0 || y < 0 || x >= _Width || y >= _Height || GetFrame(x, y) != kFrameValueUnset)
    {
        return;
    }
    
    if ((_Image[(y*_Width*4)+(x*4)+3] < 100))
    {
        SetFrame(x, y, kFrameValueEmpty);
        GenerateFrame(x+1, y);
        GenerateFrame(x, y+1);
        GenerateFrame(x-1, y);
        GenerateFrame(x, y-1);
    } else {
        SetFrame(x, y, kFrameValueBorder);
    }
}

HullGenerator::FrameValue HullGenerator::GetFrame(int x, int y)
{
    return _Frame[GetIndex(x, y)];
}

void HullGenerator::SetFrame(int x, int y, FrameValue value)
{
    _Frame[GetIndex(x, y)] = value;
}


int HullGenerator::GetIndex(int x, int y)
{
    return (y*_Width) + (x);
}
