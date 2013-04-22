#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component/graphics/font.h"

#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/text.h"
#include "view/entity/entity.h"

PB_DEFINE_ENTITY(TextViewProperty)

TextViewProperty::TextViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : ViewProperty(scene, parent, creationEntity)
{
    _Size = 1.f;
}

TextViewProperty::~TextViewProperty()
{

}

void TextViewProperty::Initialise(const std::string &path, const SchemaItem *schemaItem)
{
    ViewProperty::Initialise(path, schemaItem);
    
    pb::FontComponent* font = CreateComponent<pb::FontComponent>();
    font->SetLayer(1);
    
    Refresh();
}

void TextViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    std::string font = visualisation->EvaluateParamString(GetProjectEntity(), "font", GetPath());
    std::string text = visualisation->EvaluateParamString(GetProjectEntity(), "text", GetPath());
    float size = visualisation->EvaluateParamFloat(GetProjectEntity(), "size", GetPath(), _Size);
    
    if (font != _Font)
    {
        pb::FontRenderer::Instance()->LoadFont(font, "/fonts/" + font);
        GetComponent<pb::FontComponent>()->SetFont(font);
        _Font = font;
    }

    if (text != _Text)
    {
        GetComponent<pb::FontComponent>()->SetText(text);
        _Text = text;
    }

    if (size != _Size)
    {
        GetComponent<pb::FontComponent>()->SetSize(size);
        _Size = size;
    }
}

pb::BoundingBox TextViewProperty::CalculateBounds()
{
    glm::vec3 size(pb::FontRenderer::Instance()->MeasureString(_Font, _Text, _Size), 0);
    size *= GetViewEntity()->GetScale();
    
    glm::vec3 center = glm::vec3(GetViewEntity()->GetPosition()[0], GetViewEntity()->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
