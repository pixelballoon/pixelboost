#pragma once

#include <string>

#include "pixelboost/maths/boundingBox.h"

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class SchemaItem;
    class ViewEntity;
    
    class ViewProperty
    {
    public:
        ViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem);
        virtual ~ViewProperty();
        
        virtual void Update(float time);
        virtual void Render(pb::RenderLayer* layer);
        
        virtual void Refresh();
        
        const std::string& GetPath();
        Uid GetUid();
        Uid GetPropertyId();
        
        void DirtyBounds();
        pb::BoundingBox GetBoundingBox();
        
    protected:
        virtual pb::BoundingBox CalculateBounds();
        
        std::string EvaluateProperty(const std::string& path, const std::string& defaultValue = "");
        
        Uid _PropertyId;
        ViewEntity* _Parent;
        std::string _Path;
        const SchemaItem* _SchemaItem;
        
        bool _BoundsDirty;
        pb::BoundingBox _BoundingBox;
    };
}
