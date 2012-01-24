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
    
    void GenerateBorders();
    bool AppendBorder(int x, int y);
    void ProcessBorder(const std::vector<Vec2>& border);
    void GenerateHulls();
    
    void DebugBorder(const std::vector<Vec2>& border);
    
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
    
    std::vector<std::vector<Vec2> > _Objects;
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
    
    // End Debug
    
    GenerateBorders();
    
    for (std::vector<std::vector<Vec2> >::iterator it = _Objects.begin(); it != _Objects.end(); ++it)
    {
        DebugBorder(*it);
    }
    
    GenerateHulls();
    
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
        
        if (y == 0 || y == _Height-1)
        {
            GenerateFrame(x-1, y);
            GenerateFrame(x+1, y);
        }
        
        if (x == 0 || x == _Width-1)
        {
            GenerateFrame(x, y-1);
            GenerateFrame(x, y+1);
        }
    }
}

void HullGenerator::GenerateBorders()
{
    for (int y=0; y<_Height; y++)
    {
        for (int x=0; x<_Width; x++)
        {
            if (GetFrame(x, y) == kFrameValueBorder)
            {
                AppendBorder(x, y);
                ProcessBorder(_Border);
            }
        }
    }
}

bool HullGenerator::AppendBorder(int x, int y)
{
    if (x < 0 || y < 0 || x >= _Width || y >= _Height)
        return false;
    
    if (GetFrame(x, y) != kFrameValueBorder)
        return false;
        
    SetFrame(x, y, kFrameValueEmpty);
    
    _Border.push_back(Vec2(x, y));
    
    if (AppendBorder(x+1, y))
        return true;
    
    if (AppendBorder(x+1, y-1))
        return true;
    
    if (AppendBorder(x, y-1))
        return true;
    
    if (AppendBorder(x-1, y-1))
        return true;
    
    if (AppendBorder(x-1, y))
        return true;
    
    if (AppendBorder(x-1, y+1))
        return true;
    
    if (AppendBorder(x, y+1))
        return true;

    if (AppendBorder(x+1, y+1))
        return true;
    
    return true;
}

void HullGenerator::DebugBorder(const std::vector<Vec2>& border)
{
    for (int y=0; y<_Height; y++)
    {
        for (int x=0; x<_Width; x++)
        {
            int i=0;
            bool hasValue = false;
            for (std::vector<Vec2>::const_iterator it = border.begin(); it != border.end(); ++it)
            {
                if ((*it)[0] == x && (*it)[1] == y)
                {
                    hasValue = true;
                    break;
                }
                i++;
            }
            
            char character[16];
            sprintf(character, "%c", (char)('A' + (i%26)));
            std::cout << (hasValue ? character : " ");
        }
        
        std::cout << std::endl;
    }
}

void HullGenerator::ProcessBorder(const std::vector<Vec2>& border)
{
    std::vector<Vec2> optimisedBorder;
    
    if (!border.size())
        return;
    
    int lastIndex = 0;
    for (int i=0; i<border.size(); i++)
    {
        Vec2 lastPosition = border[lastIndex];
        Vec2 newPosition = border[i];
        
        for (int j=lastIndex; j<i; j++)
        {
            if (j-lastIndex > 1)
            {
                optimisedBorder.push_back(Vec2(lastPosition));
                lastIndex = j;
                break;
            }
        }
    }
    
    _Border.clear();
    
    _Objects.push_back(optimisedBorder);
}

void HullGenerator::GenerateHulls()
{
    
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
