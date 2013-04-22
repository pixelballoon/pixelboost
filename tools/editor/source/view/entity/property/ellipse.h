#pragma once

#include "view/entity/property/property.h"

namespace pb
{
    class RectangleComponent;
}

class EllipseViewProperty : public ViewProperty
{
    PB_DECLARE_ENTITY
    
protected:
    EllipseViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~EllipseViewProperty();
    
public:
    virtual void Initialise(const std::string &path, const SchemaItem *schemaItem);

    virtual void Refresh();
    
protected:
    virtual pb::BoundingBox CalculateBounds();
};
