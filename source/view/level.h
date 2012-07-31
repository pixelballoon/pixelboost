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
    class Entity;
    class Project;
    class Record;
    class Struct;
    class ViewEntity;
    
    class Level : public pb::MouseHandler, public pb::Entity
    {
    public:
        Level(pb::Scene* scene);
        ~Level();
                
        void Update(float time);
        void Render(int backgroundLayer, int levelLayer);
        
        void Clear();
        
        void SetRecord(Record* record);
        
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
        void CreateEntity(pixeleditor::Entity* entity);
        void DestroyEntity(pixeleditor::Entity* entity);
        void DestroyEntity(ViewEntity* entity);
        
        void UpdateSize();
        
    private:
        virtual int GetPriority();
        virtual bool OnMouseDown(pb::MouseButton button, glm::vec2 position);
        virtual bool OnMouseUp(pb::MouseButton button, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        
        virtual void OnRecordRemoved(Project* project, Record* record);
        virtual void OnEntityAdded(Record* record, pixeleditor::Entity* entity);
        virtual void OnEntityRemoved(Record* record, pixeleditor::Entity* entity);
        
        virtual void OnPropertyChanged(Struct* structure);
        
    private:
        Record* _Record;
        pb::RectangleComponent* _LevelBounds;
        
        EntityMap _Entities;
    };
}
