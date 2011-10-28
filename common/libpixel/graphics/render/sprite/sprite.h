#pragma once

#include <map>
#include <string>

#include "libpixel/graphics/device/texture.h"
#include "libpixel/math/maths.h"

namespace libpixel
{
    
class Sprite;
    
class SpriteSheet
{
public:
    SpriteSheet();
    virtual ~SpriteSheet();
    
    bool Load(const std::string& name, bool generateMips);
    
    Sprite* GetSprite(const std::string& name);
    
public:
    unsigned long _RefCount;
    
    Texture* _Texture;
    
private:
    typedef std::map<std::string, Sprite*> SpriteMap;
    SpriteMap _Sprites;
};

class Sprite
{
public:
    Vec4 GetUV();
    
public:
    SpriteSheet* _Sheet;

    Vec2 _Dimension;
    
    Vec2 _Size;
    Vec2 _Position;
    bool _Rotated;
};
    
}
