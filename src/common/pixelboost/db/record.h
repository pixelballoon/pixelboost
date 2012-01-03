#pragma once

#include <map>
#include <vector>

#include "pixelboost/db/struct.h"

namespace pixelboostdb
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
    virtual ~Record();
    
    virtual int GetType();
    virtual void Deserialise(json::Object& container, Record* context);
    
    void AddPointer(Uid pointer, void** destination);
    
public:
    typedef std::vector<Entity*> EntityList;
    
    const EntityList& GetEntities() const;
    const Entity* GetEntity(Uid uid);
    
private:
    typedef std::map<Uid, Entity*> EntityMap;
    
    EntityList _Entities;
    EntityMap _UidMap;
    
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
