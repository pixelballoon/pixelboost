#pragma once

#include <string>

#include "pixelboost/logic/entity.h"
#include "pixelboost/maths/boundingBox.h"

namespace pixeleditor
{
    class SchemaItem;
    class ViewEntity;
    
    class ViewProperty : public pb::Entity
    {
        PB_DECLARE_ENTITY
        
    public:
        ViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem);
        virtual ~ViewProperty();
        
        virtual void Update(float time);
        virtual void Render(int layer);
        
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
}
