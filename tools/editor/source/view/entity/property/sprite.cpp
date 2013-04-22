#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/component/graphics/sprite.h"

#include "core.h"
#include "project/entity.h"
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
    
    pb::SpriteComponent* sprite = CreateComponent<pb::SpriteComponent>();
    sprite->SetLayer(1);
    
    Refresh();
}

void SpriteViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    std::string sheet = visualisation->EvaluateParamString(GetProjectEntity(), "sheet", GetPath());
    std::string sprite = visualisation->EvaluateParamString(GetProjectEntity(), "sprite", GetPath());
    
    if (sheet != _SheetName || sprite != _SpriteName)
    {
        GetComponent<pb::SpriteComponent>()->SetSprite(sheet, sprite);
        _SheetName = sheet;
        _SpriteName = sprite;
    }
}

pb::BoundingBox SpriteViewProperty::CalculateBounds()
{
    glm::vec3 size(GetComponent<pb::SpriteComponent>()->GetSize(), 0);
    size *= GetViewEntity()->GetScale();
    
    glm::vec3 center = glm::vec3(GetViewEntity()->GetPosition()[0], GetViewEntity()->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
