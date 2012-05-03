#include "pixelboost/db/entity.h"
#include "pixelboost/db/record.h"
#include "pixelboost/db/manager.h"

namespace pixelboost
{
    
bool RecordHandle::IsResolved()
{
    if (record || !uid)
        return true;
    
    return false;
}
    
Record::Record(Uid uid, const std::string& type)
    : Struct(uid, type)
{
    
}
    
Record::~Record()
{
    
}
    
const Record::EntityMap& Record::GetEntities() const
{
    return _Entities;
}

const Entity* Record::GetEntity(Uid uid)
{
    EntityMap::iterator it = _Entities.find(uid);
    
    if (it != _Entities.end())
        return it->second;
    
    return 0;
}
    
void Record::AddEntity(Entity* entity)
{
    _Entities[entity->GetUid()] = entity;
}
    
void Record::AddPointer(Uid uid, void** pointer)
{
    EntityPointer ptr;
    ptr.uid = uid;
    ptr.pointer = pointer;
    
    _Pointers.push_back(ptr);
}
    
void Record::ResolvePointers()
{
    for (PointerList::iterator it = _Pointers.begin(); it != _Pointers.end(); ++it)
    {
        EntityMap::iterator entity = _Entities.find(it->uid);
        
        if (entity != _Entities.end())
            (*it->pointer) = entity->second;
    }
    
    _Pointers.clear();
}

}
