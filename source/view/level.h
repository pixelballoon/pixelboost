#pragma once

#include <map>

#include "glm/glm.hpp"
#include "sigslot/signal.h"

#include "pixelboost/input/mouseManager.h"

#include "project/definitions.h"

namespace pb
{
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
    
    class Level : public pb::MouseHandler
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
        
    public:
        const EntityMap& GetEntities();
        ViewEntity* GetEntityById(Uid uid);
        
        sigslot::Signal1<ViewEntity*> entityAdded;
        sigslot::Signal1<ViewEntity*> entityRemoved;
        
    private:
        void CreateEntity(Uid uid);
        void CreateEntity(Entity* entity);
        void DestroyEntity(Uid uid);
        
        void UpdateSize();
        
    private:
        virtual int GetPriority();
        virtual bool OnMouseDown(pb::MouseButton button, glm::vec2 position);
        virtual bool OnMouseUp(pb::MouseButton button, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        
        virtual void OnRecordRemoved(Project* project, Record* record);
        virtual void OnEntityAdded(Record* record, Entity* entity);
        virtual void OnEntityRemoved(Record* record, Entity* entity);
        
        virtual void OnPropertyChanged(Struct* structure);
        
    private:
        pb::Scene* _Scene;
        Record* _Record;
        
        EntityMap _Entities;
        
        glm::vec2 _Size;
    };
}
