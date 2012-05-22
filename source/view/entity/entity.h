#pragma once

#include <map>
#include <string>

#include "pixelboost/math/maths.h"

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class Entity;
    class SchemaItem;
    class SchemaStruct;
    class ViewProperty;
    
    class ViewEntity
    {
    public:
        ViewEntity(Entity* entity);
        ~ViewEntity();
        
        void Update(float time);
        void Render(pb::RenderLayer* layer);
        
        Entity* GetEntity();
        Vec3 GetPosition();
        
    private:
        void AddProperty(const std::string& path, ViewProperty* property);
        
        void ParseProperties();
        void ParseStruct(const std::string& path, const SchemaStruct* schemaStruct);
        void ParseItem(const std::string& path, const SchemaItem* item);
        
    private:
        typedef std::map<std::string, ViewProperty*> PropertyMap;
        
    private:
        Entity* _Entity;
        PropertyMap _Properties;
    };
}
