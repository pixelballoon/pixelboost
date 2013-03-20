#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/resources/textureResource.h"
#include "pixelboost/logic/component/graphics/sprite.h"

#include "core.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/schema.h"
#include "view/entity/property/sprite.h"
#include "view/entity/entity.h"
#include "view.h"

PB_DEFINE_ENTITY(SpriteViewProperty)

SpriteViewProperty::SpriteViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : ViewProperty(scene, parent, creationEntity)
{
    
}

SpriteViewProperty::~SpriteViewProperty()
{
    if (_Texture)
    {
        _Texture->SignalResourceLoaded.Connect(this, &SpriteViewProperty::OnResourceLoaded);
        _Texture->SignalResourceUnloading.Connect(this, &SpriteViewProperty::OnResourceUnloading);
    }
}

void SpriteViewProperty::Initialise(const std::string &path, const SchemaItem *schemaItem)
{
    ViewProperty::Initialise(path, schemaItem);
    
    pb::SpriteComponent* sprite = CreateComponent<pb::SpriteComponent>();
    sprite->SetLayer(1);
    
    Refresh();
}

void SpriteViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    std::string sprite = visualisation->EvaluateParamString(GetProjectEntity(), "sprite", GetPath());
    
    if (sprite != _SpriteName)
    {
        _SpriteName = sprite;
        
        if (_Texture)
        {
            _Texture->SignalResourceLoaded.Connect(this, &SpriteViewProperty::OnResourceLoaded);
            _Texture->SignalResourceUnloading.Connect(this, &SpriteViewProperty::OnResourceUnloading);
        }
        
        _Texture = pb::ResourceManager::Instance()->GetPool("default")->GetResource<pb::TextureResource>("editor_images/" + _SpriteName + ".png");
        _Texture->SignalResourceLoaded.Connect(this, &SpriteViewProperty::OnResourceLoaded);
        _Texture->SignalResourceUnloading.Connect(this, &SpriteViewProperty::OnResourceUnloading);
        
        if (_Texture->GetState() == pb::kResourceStateReady)
        {
            OnResourceLoaded(_Texture.get(), false);
        }
    }
}

pb::BoundingBox SpriteViewProperty::CalculateBounds()
{
    glm::vec3 size(_Sprite.Size[0], _Sprite.Size[1], 0);
    size *= GetViewEntity()->GetScale();
    
    glm::vec3 center = glm::vec3(GetViewEntity()->GetPosition()[0], GetViewEntity()->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}

void SpriteViewProperty::OnResourceLoaded(pb::Resource* resource, bool error)
{
    if (error)
        return;
    
    if (resource == _Texture.get())
    {
        _Sprite._Texture = _Texture->GetTexture();
        _Sprite.Size = _Sprite._Texture->GetSize() / (float)Core::Instance()->GetProject()->GetConfig().pixelUnit;
        _Sprite.UvPosition = glm::vec2(0,0);
        _Sprite.UvSize = glm::vec2(1,1);
        _Sprite.Rotated = false;
        _Sprite.Offset = glm::vec2(0,0);
        
        GetComponent<pb::SpriteComponent>()->SetSprite(&_Sprite);
        
        DirtyBounds();
    }
}

void SpriteViewProperty::OnResourceUnloading(pb::Resource* resource)
{
    if (resource == _Texture.get())
    {
        _Sprite._Texture = 0;
        _Sprite.Size = glm::vec2(0,0);
        
        GetComponent<pb::SpriteComponent>()->SetSprite(0);
        
        DirtyBounds();
    }
}
