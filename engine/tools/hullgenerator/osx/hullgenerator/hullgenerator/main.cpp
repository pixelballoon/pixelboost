#include <fstream>
#include <iostream>
#include <string>

#include "glm/glm.hpp"
#include "lodepng/lodepng.h"

#include "pixelboost/data/json/writer.h"
#include "pixelboost/external/polydecomp/decomp.h"

class HullGenerator
{
public:
    HullGenerator(bool debugMode, const std::string& debugPath);
    
    bool Load(const std::string& filename);
    bool Save(const std::string& filename);
    bool Process();
    
private:
    int IsTransparent(int x, int y);
    void GenerateFrame(int x, int y);
    
    void GenerateBorders();
    bool CheckBorder(int x, int y, int angle);
    bool AppendBorder(int startX, int startY, int x, int y, int angle, bool start);
    void ProcessBorder(const std::vector<glm::vec2>& border);
    void GenerateHulls();
    
    void DebugBorder(const std::vector<glm::vec2>& border, const std::string& filename);
    void DebugFrame(const std::string& filename);
    void DebugFinal(const std::string& filename);
    
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
    std::vector<glm::vec2> _Border;
    
    std::vector<std::vector<glm::vec2> > _Hulls;
    std::vector<std::vector<glm::vec2> > _Objects;
    
    std::string _DebugPath;
    bool _DebugMode;
};

int main (int argc, const char * argv[])
{
    const char* inputLocation = argc > 1 ? argv[1] : "";
    
    const char* outputLocation = argc > 2 ? argv[2] : "";
    
    const char* debugLocation = argc > 3 ? argv[3] : "";
    
    HullGenerator hullGenerator(argc > 3, debugLocation);
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
    for (std::vector<std::vector<glm::vec2> >::iterator hullIt = _Hulls.begin(); hullIt != _Hulls.end(); ++hullIt)
    {
        json::Array hull;
        
        for (std::vector<glm::vec2>::iterator it = hullIt->begin(); it != hullIt->end(); ++it)
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
    
    if (_DebugMode)
        printf("Generate borders\n");
    
    GenerateBorders();
    
    if (_DebugMode)
        printf("Generate hulls\n");
    
    GenerateHulls();
    
    if (_DebugMode)
        DebugFinal(_DebugPath + "final.png");
    
    return true;
}

int HullGenerator::IsTransparent(int x, int y)
{
    if (x<0 || y<0 || x>=_Width || y>=_Height)
        return 1;
    
    if (_Image[(y*_Width*4)+(x*4)+3] < 100)
        return 1;
    
    return 0;
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
        unsigned char border = IsTransparent(x-1, y-1) | IsTransparent(x, y-1)<<1 | IsTransparent(x+1, y-1)<<2 | IsTransparent(x-1, y)<<3 | IsTransparent(x+1, y)<<4 | IsTransparent(x-1, y+1)<<5 | IsTransparent(x, y+1)<<6 | IsTransparent(x+1, y+1)<<7;
        
        if (border & (1<<1 | 1<<3 | 1<<4 | 1<<6))
            SetFrame(x, y, kFrameValueBorder);
        else
            SetFrame(x, y, kFrameValueImage);
    }
}

void HullGenerator::GenerateBorders()
{
    for (int x=0; x<_Width; x++)
    {
        for (int y=0; y<_Height; y++)
        {
        
            if (GetFrame(x, y) == kFrameValueBorder)
            {

                AppendBorder(x, y, x, y, 0, true);
                
                for (std::vector<glm::vec2>::iterator it = _Border.begin(); it != _Border.end(); ++it)
                {
                    SetFrame((*it)[0], (*it)[1], kFrameValueEmpty);
                    *it = glm::vec2((*it)[0], _Height-1-(*it)[1]);
                }
                std::reverse(_Border.begin(), _Border.end());
                
                ProcessBorder(_Border);
                _Border.clear();
                
                if (_DebugMode)
                {
                    char debugName[128];
                    sprintf(debugName, "%s/frame_%lu.png", _DebugPath.c_str(), _Objects.size()-1);
                    DebugFrame(debugName);
                    
                    if (_Objects.size())
                    {
                        sprintf(debugName, "%s/debug_%lu.png", _DebugPath.c_str(), _Objects.size()-1);
                        DebugBorder(_Objects[_Objects.size()-1], debugName);
                    }
                }
            }
        }
    }
}

bool HullGenerator::CheckBorder(int startX, int startY, int angle)
{
    int x, y;
    
    angle = (angle+8)%8;
    
    switch (angle)
    {
        case 0:
            x = startX+1;
            y = startY;
            break;
        case 1:
            x = startX+1;
            y = startY+1;
            break;
        case 2:
            x = startX;
            y = startY+1;
            break;
        case 3:
            x = startX-1;
            y = startY+1;
            break;
        case 4:
            x = startX-1;
            y = startY;
            break;
        case 5:
            x = startX-1;
            y = startY-1;
            break;
        case 6:
            x = startX;
            y = startY-1;
            break;
        case 7:
            x = startX+1;
            y = startY-1;
            break;
    }
    
    if (x < 0 || y < 0 || x >= _Width || y >= _Height)
        return false;
    
    if (GetFrame(x, y) != kFrameValueBorder)
        return false;
    
    return true;
}

bool HullGenerator::AppendBorder(int startX, int startY, int x, int y, int angle, bool start)
{
    if (startX == x && startY == y && !start)
        return true;
    
    _Border.push_back(glm::vec2(x, y));
    
    bool prevBorder = true;
    for (int i=angle-4; i<angle+4; i++)
    {
        bool addBorder = false;

        bool border = CheckBorder(x, y, i);
        if (!prevBorder && border)
            addBorder = true;
        
        prevBorder = border;
        
        // If there's no other option, go back to where we came from (for single pixel width lines)
        if (i == angle+3 && !addBorder)
        {
            addBorder = true;
            i = angle-4;
        }

        if (addBorder)
        {
            i = (i+8)%8;

            switch (i)
            {
                case 0:
                    return AppendBorder(startX, startY, x+1, y, i, false);
                    
                case 1:
                    return AppendBorder(startX, startY, x+1, y+1, i, false);
                
                case 2:
                    return AppendBorder(startX, startY, x, y+1, i, false);

                case 3:
                    return AppendBorder(startX, startY, x-1, y+1, i, false);
                
                case 4:
                    return AppendBorder(startX, startY, x-1, y, i, false);
                
                case 5:
                    return AppendBorder(startX, startY, x-1, y-1, i, false);
                
                case 6:
                    return AppendBorder(startX, startY, x, y-1, i, false);
                
                case 7:
                    return AppendBorder(startX, startY, x+1, y-1, i, false);
            }
            
            return true;
        }
    }
    
    return false;
}

void HullGenerator::DebugBorder(const std::vector<glm::vec2>& border, const std::string& filename)
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
        glm::vec2 pos = border[i];
        int offset = (pos[1]*_Width*4)+(pos[0]*4);
        image[offset] = (i%64)*4;
        image[offset+1] = (i%64)*4;
        image[offset+2] = (i%64)*4;
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

void HullGenerator::DebugFinal(const std::string& filename)
{
    std::vector<unsigned char> buffer;
    std::vector<unsigned char> image;
    
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
    
    //for (std::vector<std::vector<Vec2> >::iterator it = _Objects.begin(); it != _Objects.end(); ++it)
    for (std::vector<std::vector<glm::vec2> >::iterator it = _Hulls.begin(); it != _Hulls.end(); ++it)
    {
        std::vector<glm::vec2>& hull = *it;
        for (int i=0; i<hull.size(); i++)
        {
            glm::vec2 a = hull[i];
            glm::vec2 b = hull[(i+1)%hull.size()];
            
            int sx = a[0];
            int sy = a[1];
            int ex = b[0];
            int ey = b[1];
            
            if (sx > ex)
            {
                std::swap(sx, ex);
                std::swap(sy, ey);
            }
            
            int deltaX = ex - sx;
            int deltaY = ey - sy;
            int y = sy;
            int yStep = (sy > ey) ? -1 : 1;
            float e = 0;
            float deltaE = abs((float)deltaY / (float)deltaX);
            for (int x=sx; x<ex; x++)
            {
                int offset = (y*_Width*4)+(x*4);
                image[offset] = (i%64)*4;
                image[offset+1] = (i%64)*4;
                image[offset+2] = (i%64)*4;
                
                e += deltaE;
                
                if (e > 0.5)
                {
                    y += yStep;
                    e -= 1.0;
                }
            }
        }
    }
    
    LodePNG::Encoder encoder;
    encoder.encode(buffer, image, _Width, _Height);
    LodePNG::saveFile(buffer, filename);
}

void HullGenerator::ProcessBorder(const std::vector<glm::vec2>& border)
{   
    std::vector<glm::vec2> optimisedBorder = border;

    if (!optimisedBorder.size())
        return;
    
    for (int i=0; i<((int)optimisedBorder.size())-1 && optimisedBorder.size() > 3;)
    {
        glm::vec2 a = optimisedBorder[i];
        
        for (int j=i+1; j<optimisedBorder.size();)
        {
            glm::vec2 b = optimisedBorder[j];
            if (glm::length(a-b) < (float)_Width/25.f)
            {
                optimisedBorder.erase(optimisedBorder.begin() + j);
            } else {
                i=j;
                break;
            }
        }
    }
    
    for (int i=0; i<((int)optimisedBorder.size())-2 && optimisedBorder.size() > 3;)
    {
        glm::vec2 a = optimisedBorder[i];
        glm::vec2 b = optimisedBorder[i+1];
        glm::vec2 c = optimisedBorder[i+2];
        
        float angle = acos(glm::dot(glm::normalize(b-a), glm::normalize(b-c)));
        float deg = (angle/M_PI)*180.f;

        if (deg >= 120.f)
        {
            optimisedBorder.erase(optimisedBorder.begin() + (i+1));
        } else {
            i++;
        }
    }
    
    _Objects.push_back(optimisedBorder);
}

void HullGenerator::GenerateHulls()
{
    for (std::vector<std::vector<glm::vec2> >::iterator it = _Objects.begin(); it != _Objects.end(); ++it)
    {
        polydecomp::PolyDecomp::Polygon poly;
        
        std::vector<glm::vec2>& object = *it;
        
        for (std::vector<glm::vec2>::iterator it = object.begin(); it != object.end(); ++it)
        {
            poly.push_back(polydecomp::Point((*it)[0],(*it)[1]));
        }
        
        polydecomp::PolyDecomp decomp;
        std::vector<polydecomp::PolyDecomp::Polygon> polys = decomp.Decompose(poly);
        
        for (std::vector<polydecomp::PolyDecomp::Polygon>::iterator polyIt = polys.begin(); polyIt != polys.end(); ++polyIt)
        {
            std::vector<glm::vec2> hull;
            glm::vec2 lastPoint(-1,-1);
            for (polydecomp::PolyDecomp::Polygon::iterator pointIt = polyIt->begin(); pointIt != polyIt->end(); ++pointIt)
            {
                glm::vec2 currentPoint = glm::vec2(pointIt->x, pointIt->y);
                if (lastPoint != currentPoint) // Should never happen!
                {
                    hull.push_back(currentPoint);
                    lastPoint = currentPoint;
                }
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
