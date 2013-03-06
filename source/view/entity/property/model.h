#pragma once

#include "view/entity/property/property.h"

namespace pb
{
    class ModelComponent;
}

class ModelViewProperty : public ViewProperty
{
    PB_DECLARE_ENTITY
    
protected:
    ModelViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~ModelViewProperty();
    
public:
    virtual void Initialise(const std::string &path, const SchemaItem *schemaItem);
    
    virtual void Refresh();
    
protected:
    virtual pb::BoundingBox CalculateBounds();
    
private:
    std::string _Model;
    std::string _Texture;
};
