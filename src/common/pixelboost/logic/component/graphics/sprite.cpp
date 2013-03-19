#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/resources/spriteSheetResource.h"
#include "pixelboost/logic/component/graphics/sprite.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::SpriteComponent)

SpriteComponent::SpriteComponent(Entity* parent)
    : RenderableComponent<pb::SpriteRenderable>(parent)
{
    
}

SpriteComponent::~SpriteComponent()
{
    if (_SpriteSheet)
    {
        _SpriteSheet->resourceLoaded.Disconnect(this, &SpriteComponent::OnResourceLoaded);
        _SpriteSheet->resourceUnloading.Disconnect(this, &SpriteComponent::OnResourceUnloading);
    }
}

glm::vec2 SpriteComponent::GetSize()
{
    if (GetRenderable()->GetSprite())
        return GetRenderable()->GetSprite()->Size;
    
    return glm::vec2(0,0);
}

void SpriteComponent::SetSprite(const std::string& filename, const std::string& sprite, const std::string& pool)
{
    _SpriteName = sprite;
    SetSprite(0);
    
    _SpriteSheet = pb::ResourceManager::Instance()->GetPool(pool)->GetResource<pb::SpriteSheetResource>(filename);
    _SpriteSheet->resourceLoaded.Connect(this, &SpriteComponent::OnResourceLoaded);
    _SpriteSheet->resourceUnloading.Connect(this, &SpriteComponent::OnResourceUnloading);
    
    if (_SpriteSheet->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_SpriteSheet.get(), false);
    }
}

void SpriteComponent::SetSprite(Sprite* sprite)
{
    if (_SpriteSheet)
    {
        _SpriteSheet->resourceLoaded.Disconnect(this, &SpriteComponent::OnResourceLoaded);
        _SpriteSheet->resourceUnloading.Disconnect(this, &SpriteComponent::OnResourceUnloading);
        _SpriteSheet.reset();
    }
    
    GetRenderable()->SetSprite(sprite);
}

void SpriteComponent::OnResourceLoaded(ResourceHandleBase* resource, bool error)
{
    if (!error)
    {
        if (resource == _SpriteSheet.get())
        {
            GetRenderable()->SetSprite(_SpriteSheet->GetResource()->GetSpriteSheet()->GetSprite(_SpriteName));
        }
    }
}

void SpriteComponent::OnResourceUnloading(ResourceHandleBase* resource)
{
    if (resource == _SpriteSheet.get())
    {
        GetRenderable()->SetSprite(0);
    }
}
