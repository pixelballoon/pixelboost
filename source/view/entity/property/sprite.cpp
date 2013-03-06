#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
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
    
}

void SpriteViewProperty::Initialise(const std::string &path, const SchemaItem *schemaItem)
{
    ViewProperty::Initialise(path, schemaItem);
    
    CreateComponent<pb::SpriteComponent>()->SetLayer(1);
    
    Refresh();
}

void SpriteViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    std::string sprite = visualisation->EvaluateParamString(GetProjectEntity(), "sprite", GetPath());
    
    if (sprite != _Sprite)
    {
        _Sprite = sprite;
        
        if (!View::Instance()->GetSpriteRenderer()->GetSpriteSheet(_Sprite))
        {
            std::shared_ptr<pb::SpriteSheet> spriteSheet = View::Instance()->GetSpriteRenderer()->CreateSpriteSheet(sprite);
            spriteSheet->LoadSingle("editor_sprites/" + _Sprite, Core::Instance()->GetProject()->GetConfig().pixelUnit);
        }
        
        GetComponent<pb::SpriteComponent>()->SetSprite(_Sprite);
        DirtyBounds();
    }
}

pb::BoundingBox SpriteViewProperty::CalculateBounds()
{
    pb::Sprite* sprite = View::Instance()->GetSpriteRenderer()->GetSprite(_Sprite);
    
    if (!sprite)
        return pb::BoundingBox();

    glm::vec3 size(sprite->_Size[0], sprite->_Size[1], 0);
    size *= GetViewEntity()->GetScale();
    
    glm::vec3 center = glm::vec3(GetViewEntity()->GetPosition()[0], GetViewEntity()->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
