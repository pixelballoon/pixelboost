#include "pixelboost/db/entity.h"
#include "pixelboost/db/record.h"

using namespace pb;
    
bool DbRecordHandle::IsResolved()
{
    if (record || !uid)
        return true;
    
    return false;
}
    
DbRecord::DbRecord(Uid uid, Uid type, void* data)
    : DbStruct(uid, type, data)
{
    
}
    
DbRecord::~DbRecord()
{
    
}
    
const DbRecord::EntityMap& DbRecord::GetEntities() const
{
    return _Entities;
}

const DbEntity* DbRecord::GetEntity(Uid uid)
{
    EntityMap::iterator it = _Entities.find(uid);
    
    if (it != _Entities.end())
        return it->second;
    
    return 0;
}
    
void DbRecord::AddEntity(DbEntity* entity)
{
    _Entities[entity->GetUid()] = entity;
}

DbEntity* DbRecord::RemoveEntity(Uid entityId)
{
    EntityMap::iterator it = _Entities.find(entityId);
    
    if (it == _Entities.end())
        return 0;
    
    DbEntity* entity = it->second;
    
    _Entities.erase(it);
    
    return entity;
}
    
void DbRecord::AddPointer(Uid uid, void** pointer)
{
    EntityPointer ptr;
    ptr.uid = uid;
    ptr.pointer = pointer;
    
    _Pointers.push_back(ptr);
}
    
void DbRecord::ResolvePointers()
{
    for (PointerList::iterator it = _Pointers.begin(); it != _Pointers.end(); ++it)
    {
        EntityMap::iterator entity = _Entities.find(it->uid);
        
        if (entity != _Entities.end())
            (*it->pointer) = entity->second->GetData();
    }
    
    _Pointers.clear();
}
