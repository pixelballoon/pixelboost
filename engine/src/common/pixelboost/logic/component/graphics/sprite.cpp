#include "pixelboost/debug/log.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/message/color.h"
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
    GetEntity()->RegisterMessageHandler<SetColorMessage>(MessageHandler(this, &SpriteComponent::OnSetColor));
}

SpriteComponent::~SpriteComponent()
{
    GetEntity()->UnregisterMessageHandler<SetColorMessage>(MessageHandler(this, &SpriteComponent::OnSetColor));
    
    if (_SpriteSheet)
    {
        _SpriteSheet->SignalResourceLoaded.Disconnect(this, &SpriteComponent::OnResourceLoaded);
        _SpriteSheet->SignalResourceUnloading.Disconnect(this, &SpriteComponent::OnResourceUnloading);
    }
}

bool SpriteComponent::IsReady()
{
    return (_SpriteSheet->GetState() == kResourceStateReady);
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
    _SpriteSheet->SignalResourceLoaded.Connect(this, &SpriteComponent::OnResourceLoaded);
    _SpriteSheet->SignalResourceUnloading.Connect(this, &SpriteComponent::OnResourceUnloading);
    
    if (_SpriteSheet->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_SpriteSheet.get(), false);
    }
}

void SpriteComponent::SetSprite(Sprite* sprite)
{
    if (_SpriteSheet)
    {
        _SpriteSheet->SignalResourceLoaded.Disconnect(this, &SpriteComponent::OnResourceLoaded);
        _SpriteSheet->SignalResourceUnloading.Disconnect(this, &SpriteComponent::OnResourceUnloading);
        _SpriteSheet.reset();
    }
    
    GetRenderable()->SetSprite(sprite);
}

void SpriteComponent::OnSetColor(const Message& message)
{
    auto colorMessage = message.As<SetColorMessage>();
    
    GetRenderable()->SetTint(colorMessage.GetColor());
}

void SpriteComponent::OnResourceLoaded(Resource* resource, bool error)
{
    if (!error)
    {
        if (resource == _SpriteSheet.get())
        {
            Sprite* sprite = _SpriteSheet->GetSpriteSheet()->GetSprite(_SpriteName);
            
            if (!sprite)
            {
                PbLogError("pb.graphics.component.sprite", "No sprite named %s on sheet %s", _SpriteName.c_str(), _SpriteSheet->GetFilename().c_str());
            }
            
            GetRenderable()->SetSprite(sprite);
        }
    }
}

void SpriteComponent::OnResourceUnloading(Resource* resource)
{
    if (resource == _SpriteSheet.get())
    {
        GetRenderable()->SetSprite(0);
    }
}
