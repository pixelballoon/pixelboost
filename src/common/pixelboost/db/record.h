#pragma once

#include <map>
#include <vector>

#include "pixelboost/db/struct.h"

namespace pixelboost
{
    
class Entity;
class Record;
    
class RecordHandle
{
public:
    bool IsResolved();
    
    std::string name;
    Uid uid;
    
    Record* record;
};
    
class Record : public Struct
{
public:
    Record(Uid uid, Uid type, void* data);
    virtual ~Record();
    
    void AddEntity(Entity* entity);    
    void AddPointer(Uid pointer, void** destination);
    
public:
    typedef std::map<Uid, Entity*> EntityMap;
    
    const EntityMap& GetEntities() const;
    const Entity* GetEntity(Uid uid);
    
private:
    void ResolvePointers();
    
private:
    EntityMap _Entities;
    
private:
    struct EntityPointer
    {
        Uid uid;
        void** pointer;
    };
    
    typedef std::vector<EntityPointer> PointerList;
    
    PointerList _Pointers;
};
    
}
