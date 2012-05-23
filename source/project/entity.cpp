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
    , _Position(0, 0, 0)
    , _Rotation(0)
    , _Scale(1, 1, 1)
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
    json::Number& tz = transform["tz"];
    json::Number& rz = transform["rz"];
    json::Number& sx = transform["sx"];
    json::Number& sy = transform["sy"];
    json::Number& sz = transform["sz"];
    
    _Position = Vec3(tx.Value(), ty.Value(), tz.Value());
    _Rotation = (float)rz.Value();
    _Scale = Vec3(sx.Value(), sy.Value(), sz.Value());
    
    return status;
}
  
bool Entity::Save(json::Object& entity)
{
    bool status = Struct::Save(entity);

    status &= WriteTransformDataJson(entity);
    
    return status;
}
    
bool Entity::ExportJson(json::Object& entity)
{
    bool status = Struct::ExportJson(entity);
    
    status &= WriteTransformDataJson(entity);
    
    return status;
}

bool Entity::ExportLua(std::iostream &output)
{
    bool status = Struct::ExportLua(output, false);
    
    output << "," << std::endl;
    
    status &= WriteTransformDataLua(output);
    
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

const Vec3& Entity::GetPosition()
{
    return _Position;
}

void Entity::SetPosition(const Vec3& position)
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

const Vec3& Entity::GetScale()
{
    return _Scale;
}

void Entity::SetScale(const Vec3& scale)
{
    _Scale = scale;
    
    entityChanged(this);
}
    
bool Entity::WriteTransformDataJson(json::Object& entity)
{
    json::Object transform;
    transform["tx"] = json::Number(_Position[0]);
    transform["ty"] = json::Number(_Position[1]);
    transform["tz"] = json::Number(_Position[2]);
    transform["rx"] = json::Number(0);
    transform["ry"] = json::Number(0);
    transform["rz"] = json::Number(_Rotation);
    transform["sx"] = json::Number(_Scale[0]);
    transform["sy"] = json::Number(_Scale[1]);
    transform["sz"] = json::Number(_Scale[2]);
    
    entity["Transform"] = transform;
    
    return true;
}

bool Entity::WriteTransformDataLua(std::iostream& output)
{
    output << "transform = { t = {" << _Position[0] << "," << _Position[1] << "," << _Position[2] << "}, r = {0,0," << _Rotation << "}, s = {" << _Scale[0] << "," << _Scale[1] << "," << _Scale[2] << "} }";
    
    return true;
}
