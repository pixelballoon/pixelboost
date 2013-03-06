#pragma once

#include <string>

#include "pixelboost/logic/entity.h"
#include "pixelboost/maths/boundingBox.h"

class SchemaItem;
class ViewEntity;

class ViewProperty : public pb::Entity
{
    PB_DECLARE_ENTITY
    
protected:
    ViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~ViewProperty();
    
public:
    virtual void Initialise(const std::string& path, const SchemaItem* schemaItem);
    
    virtual void Refresh();
    
    ViewEntity* GetViewEntity();
    ProjectEntity* GetProjectEntity();
    
    const std::string& GetPath();
    Uid GetUid();
    Uid GetPropertyId();
    
    void DirtyBounds();
    pb::BoundingBox GetBoundingBox();
    
protected:
    virtual pb::BoundingBox CalculateBounds();
    
    Uid _PropertyId;
    std::string _Path;
    const SchemaItem* _SchemaItem;
    
    bool _BoundsDirty;
    pb::BoundingBox _BoundingBox;
};
