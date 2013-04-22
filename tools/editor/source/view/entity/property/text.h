#pragma once

#include "view/entity/property/property.h"

class TextViewProperty : public ViewProperty
{
    PB_DECLARE_ENTITY
    
protected:
    TextViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~TextViewProperty();
    
public:
    virtual void Initialise(const std::string &path, const SchemaItem *schemaItem);
    
    virtual void Refresh();
    
protected:
    virtual pb::BoundingBox CalculateBounds();
    
private:
    std::string _Font;
    std::string _Text;
    float _Size;
};
