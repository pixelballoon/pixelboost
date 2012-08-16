#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/component/graphics/sprite.h"

#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/sprite.h"
#include "view/entity/entity.h"
#include "view.h"

using namespace pixeleditor;

SpriteViewProperty::SpriteViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem)
    : ViewProperty(parent, path, schemaItem)
{
    _SpriteComponent = new pb::SpriteComponent(parent, "");
    _SpriteComponent->SetLayer(1);
    
    Refresh();
}

SpriteViewProperty::~SpriteViewProperty()
{
    _Parent->DestroyComponent(_SpriteComponent);
}

void SpriteViewProperty::Update(float time)
{
    ViewProperty::Update(time);
}

void SpriteViewProperty::Render(int layer)
{
    ViewProperty::Render(layer);
}

void SpriteViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    std::string sprite = EvaluateProperty(visualisation->GetParamValue("sprite"));
    
    if (sprite != _Sprite)
    {
        _Sprite = sprite;
        View::Instance()->LoadSprite(_Sprite);
        _SpriteComponent->SetSprite(_Sprite);
        DirtyBounds();
    }
}

pb::BoundingBox SpriteViewProperty::CalculateBounds()
{
    pb::Sprite* sprite = View::Instance()->GetSpriteRenderer()->GetSprite(_Sprite);
    
    if (!sprite)
        return pb::BoundingBox();

    glm::vec3 size(sprite->_Size[0], sprite->_Size[1], 0);
    size *= _Parent->GetScale();
    
    glm::vec3 center = glm::vec3(_Parent->GetPosition()[0], _Parent->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
