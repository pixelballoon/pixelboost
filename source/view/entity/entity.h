#pragma once

#include <map>
#include <string>

#include "glm/glm.hpp"

#include "pixelboost/math/maths.h"
#include "pixelboost/maths/boundingBox.h"

#include "project/definitions.h"

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class Entity;
    class SchemaItem;
    class SchemaStruct;
    class Struct;
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
        
    public:
        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        glm::vec3 GetScale();
        
        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void SetScale(glm::vec3 scale);
        
        void Transform(glm::vec3 positionOffset, glm::vec3 rotationOffset = glm::vec3(0,0,0), glm::vec3 scaleOffset = glm::vec3(1,1,1));
        
        void CommitTransform();
        void ResetTransform();
        
    public:
        pb::BoundingBox GetBoundingBox();
        
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
        
        void OnPropertyChanged(Struct* structure);
        
        void DirtyBounds();
        void UpdateBounds();
        
    private:
        typedef std::map<std::string, Uid> PropertyIdMap;
        typedef std::map<Uid, ViewProperty*> PropertyMap;
        
    private:
        glm::vec3 _Position;
        glm::vec3 _Rotation;
        glm::vec3 _Scale;
        
        Uid _Uid;
        Entity* _Entity;
        PropertyIdMap _PropertyIdMap;
        PropertyMap _Properties;
        
        bool _BoundsDirty;
        pb::BoundingBox _BoundingBox;
        
        friend class ViewProperty;
    };
}
