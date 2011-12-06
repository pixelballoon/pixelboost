#include "pixelboost/db/entity.h"
#include "pixelboost/db/record.h"
#include "pixelboost/db/manager.h"

namespace pixelboostdb
{
    
bool RecordHandle::IsResolved()
{
    if (record || !uid)
        return true;
    
    return false;
}
    
Record::~Record()
{
    
}

int Record::GetType()
{
    return 2; // kDbRecord
}

void Record::Deserialise(json::Object& container, Record* context)
{
    Struct::Deserialise(container, this);
    
    json::Array& entities = container["Entities"];
    
    for (json::Array::iterator it = entities.Begin(); it != entities.End(); ++it)
    {
        json::Object& entity = *it;
        json::String& type = entity["Type"];
        
        Struct* s = DatabaseManager::Instance()->Create(type.Value());
        
        if (!s)
            continue;
        
        if (s->GetType() != 1) // kDbEntity
        {
            delete s;
            continue;
        }
        
        s->Deserialise(entity, this);
        
        _Entities.push_back(static_cast<Entity*>(s));
        _UidMap[s->_Uid] = static_cast<Entity*>(s);
    }
}
    
const Record::EntityList& Record::GetEntities() const
{
    return _Entities;
}

const Entity* Record::GetEntity(Uid uid)
{
    EntityMap::iterator it = _UidMap.find(uid);
    
    if (it != _UidMap.end())
        return it->second;
    
    return 0;
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
        EntityMap::iterator entity = _UidMap.find(it->uid);
        
        if (entity != _UidMap.end())
            (*it->pointer) = entity->second;
    }
    
    _Pointers.clear();
}

}
