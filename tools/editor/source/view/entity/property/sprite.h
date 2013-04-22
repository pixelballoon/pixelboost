#pragma once

#include "view/entity/property/property.h"

class SpriteViewProperty : public ViewProperty
{
    PB_DECLARE_ENTITY
    
protected:
    SpriteViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~SpriteViewProperty();
    
public:
    virtual void Initialise(const std::string &path, const SchemaItem *schemaItem);
    
    virtual void Refresh();
    
protected:
    virtual pb::BoundingBox CalculateBounds();
    
private:
    std::string _SheetName;
    std::string _SpriteName;
};
