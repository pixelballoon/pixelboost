#pragma once

#include "view/entity/property/property.h"

namespace pb
{
    class RectangleComponent;
}

class RectangleViewProperty : public ViewProperty
{
    PB_DECLARE_ENTITY
    
protected:
    RectangleViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~RectangleViewProperty();
    
public:
    virtual void Initialise(const std::string &path, const SchemaItem *schemaItem);

    virtual void Refresh();
    
protected:
    virtual pb::BoundingBox CalculateBounds();
};
