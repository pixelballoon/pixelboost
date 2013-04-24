#pragma once

#include <map>
#include <vector>

#include "pixelboost/db/struct.h"

namespace pb
{
    
class DbEntity;
struct DbPointerBase;
class DbRecord;
    
class DbRecordHandle
{
public:
    bool IsResolved();
    
    std::string name;
    Uid uid;
    
    DbRecord* record;
};
    
class DbRecord : public DbStruct
{
public:
    DbRecord(Uid uid, Uid type, DbStructData* data);
    virtual ~DbRecord();
    
    void AddEntity(DbEntity* entity);
    DbEntity* RemoveEntity(Uid entityId);
    void AddPointer(Uid uid, DbPointerBase* pointer);
    
public:
    typedef std::map<Uid, DbEntity*> EntityMap;
    
    const EntityMap& GetEntities() const;
    const DbEntity* GetEntity(Uid uid);
    
private:
    void ResolvePointers();
    
private:
    EntityMap _Entities;
    
private:
    struct EntityPointer
    {
        Uid uid;
        DbPointerBase* pointer;
    };
    
    typedef std::vector<EntityPointer> PointerList;
    
    PointerList _Pointers;
    
    friend class Database;
};
    
}
