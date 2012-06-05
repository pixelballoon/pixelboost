#pragma once

#include <map>
#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/texture.h"
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
    
    bool LoadSingle(const std::string& fileName, bool generateMips=false);
    bool LoadSheet(const std::string& name, bool generateMips=false);
    Texture* LoadTexture(const std::string& fileName, bool generateMips=false);
    
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
    SpriteSheet* _Sheet;

    glm::vec2 _Dimension;
    
    glm::vec2 _Size;
    glm::vec2 _Position;
    bool _Rotated;
};
    
}
