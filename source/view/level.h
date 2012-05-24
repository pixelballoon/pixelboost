#pragma once

#include <map>

#include "sigslot/signal.h"

#include "pixelboost/input/touchManager.h"

#include "project/definitions.h"

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class Record;
    class ViewEntity;
    
    class Level : pb::TouchHandler
    {
    public:
        Level();
        ~Level();
                
        void Update(float time);
        void Render(pb::RenderLayer* layer);
        
        void Clear();
        
        void SetRecord(Record* record);
        
        ViewEntity* GetEntityById(Uid uid);
        
        sigslot::Signal1<ViewEntity*> entityAdded;
        sigslot::Signal1<ViewEntity*> entityRemoved;
        
    private:
        void CreateEntity(Uid uid);
        void CreateEntity(Entity* entity);
        void DestroyEntity(Uid uid);
        
    private:
        virtual void OnTouchBegin(pb::Touch* touch);
        virtual void OnTouchUpdate(pb::Touch* touch);
        virtual void OnTouchEnd(pb::Touch* touch);
        
    private:
        typedef std::map<Uid, ViewEntity*> EntityMap;
        
    private:
        Record* _Record;
        
        EntityMap _Entities;
    };
}
