#include <stdio.h>

#include "project/entity.h"
#include "project/project.h"
#include "project/property.h"
#include "project/record.h"
#include "project/schema.h"

#include "pixelboost/data/json/writer.h"

using namespace pixeleditor;

Entity::Entity(Record* record, const SchemaEntity* type)
    : Struct(record->GetProject(), type)
    , _Record(record)
    , _Position(0, 0)
    , _Rotation(0)
    , _Scale(1, 1)
{
}

Entity::~Entity()
{
}

bool Entity::Open(json::Object& entity)
{
    bool status = Struct::Open(entity);
    
    json::Object& transform = entity["Transform"];
    json::Number& tx = transform["tx"];
    json::Number& ty = transform["ty"];
    json::Number& rz = transform["rz"];
    json::Number& sx = transform["sx"];
    json::Number& sy = transform["sy"];
    
    _Position = Vec2(tx.Value(), ty.Value());
    _Rotation = (float)rz.Value();
    _Scale = Vec2(sx.Value(), sy.Value());
    
    return status;
}
  
bool Entity::Save(json::Object& entity)
{
    bool status = Struct::Save(entity);

    status &= WriteTransformData(entity);
    
    return status;
}
    
bool Entity::Export(json::Object& entity)
{
    bool status = Struct::Export(entity);
    
    status &= WriteTransformData(entity);
    
    return status;
}

Record* Entity::GetRecord()
{
    return _Record;
}
    
const Record* Entity::GetRecord() const
{
    return _Record;
}

const Vec2& Entity::GetPosition()
{
    return _Position;
}

void Entity::SetPosition(const Vec2& position)
{
    _Position = position;
    
    entityChanged(this);
}
    
float Entity::GetRotation()
{
    return _Rotation;
}
    
void Entity::SetRotation(float rotation)
{
    _Rotation = rotation;
    
    entityChanged(this);
}

const Vec2& Entity::GetScale()
{
    return _Scale;
}

void Entity::SetScale(const Vec2& scale)
{
    _Scale = scale;
    
    entityChanged(this);
}
    
bool Entity::WriteTransformData(json::Object& entity)
{
    json::Object transform;
    transform["tx"] = json::Number(_Position[0]);
    transform["ty"] = json::Number(_Position[1]);
    transform["rz"] = json::Number(_Rotation);
    transform["sx"] = json::Number(_Scale[0]);
    transform["sy"] = json::Number(_Scale[1]);
    
    entity["Transform"] = transform;
    
    return true;
}
