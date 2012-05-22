#pragma once

#include <string>

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
        
    protected:
        std::string EvaluateProperty(const std::string& path, const std::string& defaultValue = "");
        
        ViewEntity* _Parent;
        std::string _Path;
        const SchemaItem* _SchemaItem;
    };
}
