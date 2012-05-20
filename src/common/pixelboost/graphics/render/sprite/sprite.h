#pragma once

#include <map>
#include <string>

#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/math/maths.h"
#include "pixelboost/misc/pointer.h"

namespace pb
{
    
class Sprite;
    
class SpriteSheet
{
private:
    SpriteSheet();
    
public:
    static std::shared_ptr<SpriteSheet> Create();
    virtual ~SpriteSheet();
    
    bool LoadDefinition(const std::string& name, bool generateMips=false);
    
    Sprite* GetSprite(const std::string& name);
    
public:
    Texture* _Texture;
    
private:
    typedef std::map<std::string, Sprite*> SpriteMap;
    SpriteMap _Sprites;
    
    friend class SpriteRenderer;
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
