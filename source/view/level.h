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
        
    private:
        typedef std::map<Uid, ViewEntity*> EntityMap;
        
    private:
        EntityMap _Entities;
    };
}
