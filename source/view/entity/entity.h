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
        ViewEntity(Uid uid, Entity* entity);
        ~ViewEntity();
        
        void Update(float time);
        void Render(pb::RenderLayer* layer);
        
        Uid GetUid();
        
        Entity* GetEntity();
        Vec3 GetPosition();
        
        Uid GetPropertyIdByPath(const std::string& path);
        ViewProperty* GetPropertyByPath(const std::string& path);
        ViewProperty* GetPropertyById(Uid uid);

    private:
        Uid AddProperty(ViewProperty* property);
        void RemoveProperty(ViewProperty* property);
        void RemovePropertyById(Uid uid);
        
        Uid GeneratePropertyId(const std::string& path);
        
        void ParseProperties();
        void ParseStruct(const std::string& path, const SchemaStruct* schemaStruct);
        void ParseItem(const std::string& path, const SchemaItem* item);
        
    private:
        typedef std::map<std::string, Uid> PropertyIdMap;
        typedef std::map<Uid, ViewProperty*> PropertyMap;
        
    private:
        Uid _Uid;
        Entity* _Entity;
        PropertyIdMap _PropertyIdMap;
        PropertyMap _Properties;
        
        friend class ViewProperty;
    };
}
