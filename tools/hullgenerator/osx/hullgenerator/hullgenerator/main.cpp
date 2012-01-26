#include <fstream>
#include <iostream>
#include <string>

#include "pixelboost/data/json/writer.h"
#include "pixelboost/math/maths.h"
#include "pixelboost/external/lodepng/lodepng.h"
#include "pixelboost/external/polydecomp/decomp.h"

class HullGenerator
{
public:
    HullGenerator(bool debugMode, const std::string& debugPath);
    
    bool Load(const std::string& filename);
    bool Save(const std::string& filename);
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
    
    std::vector<std::vector<Vec2> > _Hulls;
    std::vector<std::vector<Vec2> > _Objects;
    
    std::string _DebugPath;
    bool _DebugMode;
};

int main (int argc, const char * argv[])
{
    const char* inputLocation = argc > 1 ? argv[1] : "/Users/aeonflame/Development/moshenltd/dragonsdream/resources/spritesheets/images/blockers/ArchesMorning_bottom_3.png";
    
    const char* outputLocation = argc > 2 ? argv[2] : "/Users/aeonflame/Development/moshenltd/dragonsdream/data/physics/ArchesMorning_bottom_3.psy";
    
    HullGenerator hullGenerator(true, "/Users/aeonflame/Development/moshenltd/dragonsdream/debug/");
    hullGenerator.Load(inputLocation);
    hullGenerator.Process();
    hullGenerator.Save(outputLocation);

    return 0;
}

HullGenerator::HullGenerator(bool debugMode, const std::string& debugPath)
    : _DebugMode(debugMode)
    , _DebugPath(debugPath)
{
    
}

bool HullGenerator::Load(const std::string& filename)
{
    std::vector<unsigned char> buffer;
    
    LodePNG::loadFile(buffer, filename);
    LodePNG::Decoder decoder;
    
    decoder.decode(_Image, buffer.size() ? &buffer[0] : 0, (unsigned)buffer.size());
    
    if(decoder.hasError())
    {
        std::cout << "error " << decoder.getError() << ": "<< LodePNG_error_text(decoder.getError()) << std::endl;
        return false;
    }
    
    _Width = decoder.getWidth();
    _Height = decoder.getHeight();
    
    return true;
}

bool HullGenerator::Save(const std::string& filename)
{
    float heightOffset = _Height/32.f/2.f;
    float widthOffset = _Width/32.f/2.f;
    
    json::Object o;
    
    json::Array hulls;
    for (std::vector<std::vector<Vec2> >::iterator hullIt = _Hulls.begin(); hullIt != _Hulls.end(); ++hullIt)
    {
        json::Array hull;
        
        for (std::vector<Vec2>::iterator it = hullIt->begin(); it != hullIt->end(); ++it)
        {
            json::Object pt;
            pt["x"] = json::Number((*it)[0]/32.f-widthOffset);
            pt["y"] = json::Number((*it)[1]/32.f-heightOffset);
            hull.Insert(pt);
        }
        
        hulls.Insert(hull);
    }
    
    o["hulls"] = hulls;
    
    std::ofstream file;
    file.open(filename.c_str());
    
    if (!file.is_open())
        return false;
    
    json::Writer::Write(o, file);
    file.close();
    
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
        DebugFrame(_DebugPath + "main.png");
    
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
                    sprintf(debugName, "%s/frame_%lu.png", _DebugPath.c_str(), _Objects.size()-1);
                    DebugFrame(debugName);
                    
                    sprintf(debugName, "%s/debug_%lu.png", _DebugPath.c_str(), _Objects.size()-1);
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
            image.push_back(255);
            image.push_back(255);
            image.push_back(255);
            image.push_back(255);
        }
    }
    
    for (int i=0; i<border.size(); i++)
    {
        Vec2 pos = border[i];
        int offset = (pos[1]*_Width*4)+(pos[0]*4);
        image[offset] = 0;
        image[offset+1] = 0;
        image[offset+2] = 0;
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
            if (j-lastIndex > 20)
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
    for (std::vector<std::vector<Vec2> >::iterator it = _Objects.begin(); it != _Objects.end(); ++it)
    {
        polydecomp::PolyDecomp::Polygon poly;
        
        std::vector<Vec2>& object = *it;
        
        for (std::vector<Vec2>::iterator it = object.begin(); it != object.end(); ++it)
        {
            poly.push_back(polydecomp::Point((*it)[0],(*it)[1]));
        }
        
        polydecomp::PolyDecomp decomp;
        std::vector<polydecomp::PolyDecomp::Polygon> polys = decomp.Decompose(poly);
        
        for (std::vector<polydecomp::PolyDecomp::Polygon>::iterator polyIt = polys.begin(); polyIt != polys.end(); ++polyIt)
        {
            std::vector<Vec2> hull;
            for (polydecomp::PolyDecomp::Polygon::iterator pointIt = polyIt->begin(); pointIt != polyIt->end(); ++pointIt)
            {
                hull.push_back(Vec2(pointIt->x, pointIt->y));
            }
            _Hulls.push_back(hull);
            
            if (_DebugMode)
            {
                char debugName[128];
                sprintf(debugName, "%s/hull_%lu.png", _DebugPath.c_str(), _Hulls.size()-1);
                DebugBorder(_Hulls[_Hulls.size()-1], debugName);
            }
        }
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
