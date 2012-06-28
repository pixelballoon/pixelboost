#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/data/json/reader.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/helper/screenHelpers.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"

using namespace pb;
    
std::shared_ptr<SpriteSheet> SpriteSheet::Create()
{
    return std::shared_ptr<SpriteSheet>(new SpriteSheet());
}

SpriteSheet::SpriteSheet()
    : _Texture(0)
{
}

SpriteSheet::~SpriteSheet()
{
    for (SpriteMap::iterator it = _Sprites.begin(); it != _Sprites.end(); ++it)
    {
        Game::Instance()->GetSpriteRenderer()->_Sprites.erase(it->first);
    }
    
    if (_Texture)
    {
        GraphicsDevice::Instance()->DestroyTexture(_Texture);
    }
}

bool SpriteSheet::LoadSingle(const std::string& fileName, bool generateMips)
{
    if (!LoadTexture(fileName))
        return false;
    
    std::string spriteName = fileName.substr(fileName.rfind("/")+1);
    spriteName = spriteName.substr(0, spriteName.length()-4);
    
    Sprite* sprite = new Sprite();
    sprite->_Sheet = this;
    sprite->_Rotated = false;
    sprite->_Size = _Texture->GetSize() / ScreenHelpers::GetDpu();
    sprite->_UvPosition = glm::vec2(0,0);
    sprite->_UvSize = glm::vec2(1,1);
    sprite->_Offset = glm::vec2(0,0);
    
    _Sprites[spriteName] = sprite;
    Game::Instance()->GetSpriteRenderer()->_Sprites[spriteName] = sprite;
    
    return true;
}

bool SpriteSheet::LoadSheet(const std::string& name, bool generateMips)
{
    std::string fileRoot = FileHelpers::GetRootPath();
    
    std::string jsonFilename = fileRoot + "/data/spritesheets/" + name + (ScreenHelpers::IsHighResolution() ? "-hd" : "") + ".json";
    
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
        json::Number frameX = frame["x"];
        json::Number frameY = frame["y"];
        json::Number frameW = frame["w"];
        json::Number frameH = frame["h"];
        
        json::Object spriteSourceSize = data["spriteSourceSize"];
        json::Number spriteSourceSizeX = spriteSourceSize["x"];
        json::Number spriteSourceSizeY = spriteSourceSize["y"];
        json::Number spriteSourceSizeW = spriteSourceSize["w"];
        json::Number spriteSourceSizeH = spriteSourceSize["h"];
        
        json::Object sourceSize = data["sourceSize"];
        json::Number sourceSizeW = sourceSize["w"];
        json::Number sourceSizeH = sourceSize["h"];
        
        json::Boolean rotated = data["rotated"];
        
        Sprite* sprite = new Sprite();
        
        sprite->_Sheet = this;
        
        sprite->_Size = glm::vec2(frameW.Value(), frameH.Value()) / ScreenHelpers::GetDpu();
        sprite->_Offset = glm::vec2(sourceSizeW.Value()/2.f-spriteSourceSizeX.Value()-spriteSourceSizeW.Value()/2.f, -(sourceSizeH.Value()/2.f-spriteSourceSizeY.Value()-spriteSourceSizeH.Value()/2.f)) / ScreenHelpers::GetDpu();
        
        sprite->_Rotated = rotated.Value();
        
        sprite->_UvPosition = glm::vec2(frameX.Value()/width.Value(), frameY.Value()/height.Value());
        sprite->_UvSize = sprite->_Rotated ? glm::vec2(frameW.Value()/height.Value(), frameH.Value()/width.Value()) : glm::vec2(frameW.Value()/width.Value(), frameH.Value()/height.Value());
        
        std::string spriteName = name.substr(0, name.length()-4);
        _Sprites[spriteName] = sprite;
        Game::Instance()->GetSpriteRenderer()->_Sprites[spriteName] = sprite;
    }
    
    LoadTexture(fileRoot + "/data/spritesheets/images/" + name + (ScreenHelpers::IsHighResolution() ? "-hd" : "") + ".png", generateMips);
    
    return true;
}

Texture* SpriteSheet::LoadTexture(const std::string& fileName, bool generateMips)
{
    if (_Texture)
    {
        GraphicsDevice::Instance()->DestroyTexture(_Texture);
    }
    
    _Texture = GraphicsDevice::Instance()->CreateTexture();
    _Texture->LoadFromPng(fileName, generateMips);
    
    return _Texture;
}
    
Sprite* SpriteSheet::GetSprite(const std::string& name)
{
    SpriteMap::iterator it = _Sprites.find(name);
    
    if (it == _Sprites.end())
        return 0;
    
    return it->second;
}

#endif
