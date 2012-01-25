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
    HullGenerator(bool debugMode);
    
    bool Load(const std::string& filename);
    bool Process();
    
private:
    bool IsTransparent(int x, int y);
    void GenerateFrame(int x, int y);
    
    void GenerateBorders();
    bool AppendBorder(int x, int y);
    void ProcessBorder(const std::vector<Vec2>& border);
    void GenerateHulls();
    
    void DebugBorder(const std::vector<Vec2>& border, const std::string& filename);
    void DebugFrame(const std::string& filename);
    
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
    
    bool _DebugMode;
};

int main (int argc, const char * argv[])
{
    const char* filename = argc > 1 ? argv[1] : "default.png";
    
    HullGenerator hullGenerator(false);
    hullGenerator.Load(filename);
    hullGenerator.Process();

    return 0;
}

HullGenerator::HullGenerator(bool debugMode)
    : _DebugMode(debugMode)
{
    
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
    
    for (int x=0; x<_Width; x++)
    {
        for (int y=0; y<_Height; y++)
        {
            GenerateFrame(x, y);
        }
    }

    
    if (_DebugMode)
        DebugFrame("/Users/aeonflame/Development/moshenltd/dragonsdream/debug/main.png");
    
    GenerateBorders();
    
    GenerateHulls();
    
    return true;
}

bool HullGenerator::IsTransparent(int x, int y)
{
    if (x<0 || y<0 || x>=_Width || y>=_Height)
        return true;
    
    if (_Image[(y*_Width*4)+(x*4)+3] < 100)
        return true;
    
    return false;
}

void HullGenerator::GenerateFrame(int x, int y)
{
    if (x < 0 || y < 0 || x >= _Width || y >= _Height || GetFrame(x, y) != kFrameValueUnset)
    {
        return;
    }
    
    if (IsTransparent(x, y))
    {
        SetFrame(x, y, kFrameValueEmpty);
    } else {
        if (IsTransparent(x-1, y) || IsTransparent(x, y-1) || IsTransparent(x, y+1) || IsTransparent(x+1, y))
        {
            SetFrame(x, y, kFrameValueBorder);
        } else {
            SetFrame(x, y, kFrameValueImage);
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
                _Border.clear();
                
                if (_DebugMode)
                {
                    char debugName[128];
                    sprintf(debugName, "/Users/aeonflame/Development/moshenltd/dragonsdream/debug/frame_%lu.png", _Objects.size()-1);
                    DebugFrame(debugName);
                    
                    sprintf(debugName, "/Users/aeonflame/Development/moshenltd/dragonsdream/debug/debug_%lu.png", _Objects.size()-1);
                    DebugBorder(_Objects[_Objects.size()-1], debugName);
                }
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
    
    if (AppendBorder(x+1, y+1))
        return true;
    
    if (AppendBorder(x, y+1))
        return true;
    
    if (AppendBorder(x-1, y+1))
        return true;
    
    if (AppendBorder(x-1, y))
        return true;
    
    if (AppendBorder(x-1, y-1))
        return true;
    
    if (AppendBorder(x, y-1))
        return true;

    if (AppendBorder(x+1, y-1))
        return true;
    
    return true;
}

void HullGenerator::DebugBorder(const std::vector<Vec2>& border, const std::string& filename)
{
    std::vector<unsigned char> image;
    std::vector<unsigned char> buffer;
    
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
            
            image.push_back(hasValue ? 0 : 255);
            image.push_back(hasValue ? 0 : 255);
            image.push_back(hasValue ? 0 : 255);
            image.push_back(255);
        }
    }
    
    LodePNG::Encoder encoder;
    encoder.encode(buffer, image, _Width, _Height);
    LodePNG::saveFile(buffer, filename);
}

void HullGenerator::DebugFrame(const std::string& filename)
{
    std::vector<unsigned char> image;
    std::vector<unsigned char> buffer;
    
    for (int y=0; y<_Height; y++)
    {
        for (int x=0; x<_Width; x++)
        {
            char value;
            
            if (GetFrame(x, y) == kFrameValueBorder)
                value = 0;
            else if (GetFrame(x, y) == kFrameValueImage)
                value = 230;
            else
                value = 255;
            
            image.push_back(value);
            image.push_back(value);
            image.push_back(value);
            image.push_back(255);
        }
    }
    
    LodePNG::Encoder encoder;
    encoder.encode(buffer, image, _Width, _Height);
    LodePNG::saveFile(buffer, filename);
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
            //if (j-lastIndex > 1)
            {
                optimisedBorder.push_back(Vec2(lastPosition));
                lastIndex = i;
                break;
            }
        }
    }
    
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
