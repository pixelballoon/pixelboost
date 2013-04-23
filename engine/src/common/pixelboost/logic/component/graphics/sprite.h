#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/component/graphics/renderable.h"
#include "pixelboost/resource/resourceManager.h"

namespace pb
{

    class Message;
    class SpriteSheetResource;

    class SpriteComponent : public RenderableComponent<SpriteRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        SpriteComponent(Entity* parent);
        virtual ~SpriteComponent();
        
    public:
        bool IsReady();
        
        glm::vec2 GetSize();
        
        void SetSprite(const std::string& file, const std::string& sprite, const std::string& pool = "default");
        void SetSprite(Sprite* sprite);
        
    private:
        void OnSetColor(const Message& message);
        
        void OnResourceLoaded(Resource* resource, bool loaded);
        void OnResourceUnloading(Resource* resource);
        
        std::shared_ptr<SpriteSheetResource> _SpriteSheet;
        std::string _SpriteName;
    };
        
}
