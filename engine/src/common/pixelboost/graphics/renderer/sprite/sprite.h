#pragma once

#include <map>
#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/texture.h"

namespace pb
{
    
    struct Sprite;
        
    class SpriteSheet
    {
    public:
        SpriteSheet();
        virtual ~SpriteSheet();
        
        void AddSprite(const std::string& name, glm::vec2 size, glm::vec2 offset, glm::vec2 uvSize, glm::vec2 uvPosition, bool rotated);
        Sprite* GetSprite(const std::string& name);
        
        void SetTexture(Texture* texture);
        Texture* GetTexture();

    private:
        std::map<std::string, Sprite> _Sprites;
        Texture* _Texture;
        
        friend class SpriteRenderer;
    };

    struct Sprite
    {
        Sprite();
        Sprite(Texture* texture, glm::vec2 size, glm::vec2 offset, glm::vec2 uvSize, glm::vec2 uvPosition, bool rotated);
        
        Texture* _Texture;

        glm::vec2 Size;
        glm::vec2 Offset;
        
        glm::vec2 UvSize;
        glm::vec2 UvPosition;
        bool Rotated;
    };
    
}
