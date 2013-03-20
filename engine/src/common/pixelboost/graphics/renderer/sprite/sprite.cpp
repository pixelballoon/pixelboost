#include "json/reader.h"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"

using namespace pb;
    
SpriteSheet::SpriteSheet()
    : _Texture(0)
{

}

SpriteSheet::~SpriteSheet()
{

}

void SpriteSheet::AddSprite(const std::string& name, glm::vec2 size, glm::vec2 offset, glm::vec2 uvSize, glm::vec2 uvPosition, bool rotated)
{
    PbAssert(_Sprites.find(name) == _Sprites.end());
    
    _Sprites[name] = Sprite(_Texture, size, offset, uvSize, uvPosition, rotated);
}

Sprite* SpriteSheet::GetSprite(const std::string& name)
{
    auto it = _Sprites.find(name);
    
    if (it == _Sprites.end())
        return 0;
    
    return &it->second;
}

void SpriteSheet::SetTexture(Texture* texture)
{
    _Texture = texture;
    
    for (auto& sprite : _Sprites)
    {
        sprite.second._Texture = texture;
    }
}

Texture* SpriteSheet::GetTexture()
{
    return _Texture;
}

Sprite::Sprite()
{
    _Texture = 0;
    Rotated = false;
}

Sprite::Sprite(Texture* texture, glm::vec2 size, glm::vec2 offset, glm::vec2 uvSize, glm::vec2 uvPosition, bool rotated)
{
    _Texture = texture;
    Size = size;
    Offset = offset;
    UvSize = uvSize;
    UvPosition = uvPosition;
    Rotated = rotated;
}
