#pragma once

#include "project/definitions.h"
#include <map>

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class Record;
    class ViewEntity;
    
    class Level
    {
    public:
        Level();
        ~Level();
                
        void Update(float time);
        void Render(pb::RenderLayer* layer);
        
        void Clear();
        
        void SetRecord(Record* record);
        
        Uid GenerateViewEntityId(Entity* entity);
        
        ViewEntity* GetEntityByEntityId(Uid uid);
        ViewEntity* GetEntityByViewEntityId(Uid uid);
        
    private:
        typedef std::map<Uid, Uid> EntityIdMap;
        typedef std::map<Uid, ViewEntity*> EntityMap;
        
    private:
        EntityIdMap _EntityIdMap;
        EntityMap _Entities;
    };
}
