#include "libpixel/file/fileHelpers.h"
#include "libpixel/graphics/device/texture.h"
#include "libpixel/graphics/helper/screenHelpers.h"
#include "libpixel/graphics/render/sprite/sprite.h"
#include "libpixel/json/reader.h"
#include "libpixel/logic/game.h"

namespace libpixel
{

SpriteSheet::SpriteSheet()
{
}

SpriteSheet::~SpriteSheet()
{
}

bool SpriteSheet::Load(const std::string& name, bool generateMips)
{
    std::string fileRoot = FileHelpers::GetRootPath();
    
    std::string jsonFilename = fileRoot + "/data/spritesheets/" + name + (ScreenHelpers::IsHighResolution() && ScreenHelpers::UseHdGraphics() ? "-hd" : "") + ".json";
    
    std::string rootData = FileHelpers::FileToString(jsonFilename);
    
    json::Object root;
    
    json::Reader::Read(root, rootData);
    
    json::Object& meta = root["meta"];
    
    json::Object& size = meta["size"];
    json::Number& width = size["w"];
    json::Number& height = size["h"];
    
    json::Object& frames = root["frames"];
    
    for (json::Object::iterator it = frames.Begin(); it != frames.End(); ++it)
    {
        std::string name = it->name;
        json::Object data = it->element;
        
        json::Object frame = data["frame"];
        
        json::Number posX = frame["x"];
        json::Number posY = frame["y"];
        json::Number sizeX = frame["w"];
        json::Number sizeY = frame["h"];
        
        json::Boolean rotated = data["rotated"];
        
        Sprite* sprite = new Sprite();
        
        sprite->_Sheet = this;
        
        float ppu = 16.f;
        if (ScreenHelpers::IsHighResolution())
            ppu = 32.f;
        
        sprite->_Dimension = Vec2(sizeX.Value(), sizeY.Value()) / ppu;
        
        sprite->_Position = Vec2(posX.Value()/width.Value(), posY.Value()/height.Value());
        sprite->_Size = Vec2(sizeX.Value()/width.Value(), sizeY.Value()/height.Value());
        
        sprite->_Rotated = rotated.Value();
        
        sprite->_Scale = (ScreenHelpers::IsHighResolution() && !ScreenHelpers::UseHdGraphics()) ? Vec2(2, 2) : Vec2(1,1);
        
        _Sprites[name.substr(0, name.length()-4)] = sprite;
    }
    
    _Texture = LoadTexture(fileRoot + "/data/spritesheets/images/" + name + (ScreenHelpers::IsHighResolution() && ScreenHelpers::UseHdGraphics() ? "-hd" : "") + ".png", generateMips);
    
    return (_Texture != 0);
}
    
Sprite* SpriteSheet::GetSprite(const std::string& name)
{
    SpriteMap::iterator it = _Sprites.find(name);
    
    if (it == _Sprites.end())
        return 0;
    
    return it->second;
}

}
