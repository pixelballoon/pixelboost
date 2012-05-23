#include "pixelboost/graphics/render/sprite/sprite.h"
#include "pixelboost/graphics/render/sprite/spriteRenderer.h"

#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/sprite.h"
#include "view/entity/entity.h"
#include "view.h"

using namespace pixeleditor;

SpriteViewProperty::SpriteViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem)
    : ViewProperty(parent, path, schemaItem)
{
    Refresh();
}

SpriteViewProperty::~SpriteViewProperty()
{
    
}

void SpriteViewProperty::Update(float time)
{
    ViewProperty::Update(time);
}

void SpriteViewProperty::Render(pb::RenderLayer* layer)
{
    View::Instance()->GetSpriteRenderer()->AttachSprite(layer, _Sprite, Vec2(_Parent->GetPosition()[0], _Parent->GetPosition()[1]));
    
    ViewProperty::Render(layer);
}

void SpriteViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    _Sprite = EvaluateProperty(visualisation->GetParamValue("sprite"));
    View::Instance()->LoadSprite(_Sprite);
}

pb::BoundingBox SpriteViewProperty::CalculateBounds()
{
    pb::Sprite* sprite = View::Instance()->GetSpriteRenderer()->GetSprite(_Sprite);
    
    if (!sprite)
        return pb::BoundingBox();

    glm::vec3 size(sprite->_Dimension[0], sprite->_Dimension[1], 0);
    glm::vec3 center = glm::vec3(_Parent->GetPosition()[0], _Parent->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
