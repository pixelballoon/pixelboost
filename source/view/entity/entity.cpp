#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"

#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/property.h"
#include "view/entity/property/sprite.h"
#include "view/entity/entity.h"
#include "view.h"

using namespace pixeleditor;

ViewEntity::ViewEntity(Entity* entity)
    : _Entity(entity)
{
    ParseProperties();
}

ViewEntity::~ViewEntity()
{
    
}

void ViewEntity::Update(float time)
{
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->Update(time);
    }
}

void ViewEntity::Render(pb::RenderLayer* layer)
{
    Vec3 position = _Entity->GetPosition();
    
    View::Instance()->GetPrimitiveRenderer()->AttachBox(layer, Vec2(position[0], position[1]), Vec2(0.1,0.1));
    
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->Render(layer);
    }
}

Entity* ViewEntity::GetEntity()
{
    return _Entity;
}

Vec3 ViewEntity::GetPosition()
{
    return _Entity->GetPosition();
}

void ViewEntity::AddProperty(const std::string& path, ViewProperty* property)
{
    PropertyMap::iterator it = _Properties.find(path);
    if (it != _Properties.end())
    {
        delete it->second;
        _Properties.erase(it);
    }
    
    _Properties[path] = property;
}

void ViewEntity::ParseProperties()
{
    const SchemaStruct* schemaStruct = _Entity->GetType();
    
    ParseItem("/", schemaStruct);
    ParseStruct("/", schemaStruct);
}

void ViewEntity::ParseStruct(const std::string& path, const SchemaStruct* schemaStruct)
{
    for (SchemaStruct::PropertyMap::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        std::string propertyPath = path + it->second->GetName() + "/";
        
        ParseItem(propertyPath, it->second);
        
        switch (it->second->GetPropertyType())
        {
            case SchemaProperty::kSchemaPropertyAtom:
            {
                break;
            }
            case SchemaProperty::kSchemaPropertyArray:
            {
                break;
            }
            case SchemaProperty::kSchemaPropertyStruct:
            {
                SchemaPropertyStruct* propertyStruct = static_cast<SchemaPropertyStruct*>(it->second);
                ParseStruct(propertyPath, propertyStruct->GetSchemaStruct());
                break;
            }
            case SchemaProperty::kSchemaPropertyPointer:
            {
                break;
            }
        }
    }
}

void ViewEntity::ParseItem(const std::string& path, const SchemaItem* item)
{
    if (const SchemaAttribute* visualisation = item->GetAttribute("Visualisation"))
    {
        std::string type = visualisation->GetParamValue("type");
        
        if (type == "sprite")
        {
            SpriteViewProperty* property = new SpriteViewProperty(this, path, item);
            AddProperty(path, property);
        }
    }
}
