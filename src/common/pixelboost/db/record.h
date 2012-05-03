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
    Record(Uid uid, const std::string& type);
    virtual ~Record();
    
    void AddEntity(Entity* entity);    
    void AddPointer(Uid pointer, void** destination);
    
public:
    typedef std::map<Uid, Entity*> EntityMap;
    
    const EntityMap& GetEntities() const;
    const Entity* GetEntity(Uid uid);
    
private:
    EntityMap _Entities;
    
protected:
    struct EntityPointer
    {
        Uid uid;
        void** pointer;
    };
    
    void ResolvePointers();
    
    typedef std::vector<EntityPointer> PointerList;
    
    PointerList _Pointers;
};
    
}
