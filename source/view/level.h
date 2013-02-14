#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"
#include "sigslot/signal.h"

#include "pixelboost/logic/entity.h"
#include "pixelboost/input/mouseManager.h"

#include "project/definitions.h"

namespace pb
{
    class BoundingBox;
    class RectangleComponent;
    class RenderLayer;
    class Scene;
}

namespace pixeleditor
{
    
    class Project;
    class ProjectEntity;
    class ProjectRecord;
    class ProjectStruct;
    class Property;
    class ViewEntity;
    
    class Level : public pb::MouseHandler, public pb::Entity
    {
    public:
        Level(pb::Scene* scene);
        ~Level();
        
        virtual pb::Uid GetType() const;
        static pb::Uid GetStaticType();
                
        void Update(float time);
        
        void Clear();
        
        void SetRecord(ProjectRecord* record);
        
    public:
        typedef std::map<Uid, ViewEntity*> EntityMap;
        typedef std::vector<ViewEntity*> EntityList;
        
    public:
        const EntityMap& GetEntities();
        ViewEntity* GetEntityById(Uid uid);
        EntityList GetEntitiesInBounds(const pb::BoundingBox& bounds);
        
        sigslot::Signal1<ViewEntity*> entityAdded;
        sigslot::Signal1<ViewEntity*> entityRemoved;
        
    private:
        void CreateEntity(Uid uid);
        void CreateEntity(ProjectEntity* entity);
        void DestroyEntity(ProjectEntity* entity);
        void DestroyEntity(ViewEntity* entity);
        
        void UpdateSize();
        
    private:
        virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifier, glm::vec2 position);
        virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifier, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        
        virtual void OnRecordRemoved(Project* project, ProjectRecord* record);
        virtual void OnEntityAdded(ProjectRecord* record, ProjectEntity* entity);
        virtual void OnEntityRemoved(ProjectRecord* record, ProjectEntity* entity);
        
        virtual void OnPropertyChanged(ProjectStruct* structure, Property* property);
        
    private:
        ProjectRecord* _Record;
        pb::RectangleComponent* _LevelBounds;
        
        EntityMap _Entities;
    };
    
}
