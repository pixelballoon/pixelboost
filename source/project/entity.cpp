#include <stdio.h>

#include "project/entity.h"
#include "project/project.h"
#include "project/property.h"
#include "project/record.h"
#include "project/schema.h"

#include "pixelboost/data/json/writer.h"

using namespace pixeleditor;

ProjectEntity::ProjectEntity(ProjectRecord* record, const SchemaEntity* type)
    : ProjectStruct(record->GetProject(), type)
    , _Record(record)
    , _Position(0, 0, 0)
    , _Rotation(0)
    , _Scale(1, 1, 1)
{

}

ProjectEntity::~ProjectEntity()
{

}

bool ProjectEntity::Open(json::Object& entity, bool skipUid)
{
    bool status = ProjectStruct::Open(entity, skipUid);
    
    json::Object& transform = entity["Transform"];
    json::Number& tx = transform["tx"];
    json::Number& ty = transform["ty"];
    json::Number& tz = transform["tz"];
    json::Number& rz = transform["rz"];
    json::Number& sx = transform["sx"];
    json::Number& sy = transform["sy"];
    json::Number& sz = transform["sz"];
    
    _Position = glm::vec3(tx.Value(), ty.Value(), tz.Value());
    _Rotation = (float)rz.Value();
    _Scale = glm::vec3(sx.Value(), sy.Value(), sz.Value());
    
    return status;
}
  
bool ProjectEntity::Save(json::Object& entity)
{
    bool status = ProjectStruct::Save(entity);

    status &= WriteTransformDataJson(entity);
    
    return status;
}
    
bool ProjectEntity::ExportJson(json::Object& entity)
{
    bool status = ProjectStruct::ExportJson(entity);
    
    status &= WriteTransformDataJson(entity);
    
    return status;
}

bool ProjectEntity::ExportLua(std::iostream &output)
{
    bool status = ProjectStruct::ExportLua(output, false);
    
    output << "," << std::endl;
    
    status &= WriteTransformDataLua(output);
    
    return status;
}

ProjectRecord* ProjectEntity::GetRecord()
{
    return _Record;
}
    
const ProjectRecord* ProjectEntity::GetRecord() const
{
    return _Record;
}

const glm::vec3& ProjectEntity::GetPosition()
{
    return _Position;
}

void ProjectEntity::SetPosition(const glm::vec3& position)
{
    _Position = position;
    
    entityChanged(this);
}
    
float ProjectEntity::GetRotation()
{
    return _Rotation;
}
    
void ProjectEntity::SetRotation(float rotation)
{
    _Rotation = rotation;
    
    entityChanged(this);
}

const glm::vec3& ProjectEntity::GetScale()
{
    return _Scale;
}

void ProjectEntity::SetScale(const glm::vec3& scale)
{
    _Scale = scale;
    
    entityChanged(this);
}
    
bool ProjectEntity::WriteTransformDataJson(json::Object& entity)
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

bool ProjectEntity::WriteTransformDataLua(std::iostream& output)
{
    output << "transform = { t = {" << _Position[0] << "," << _Position[1] << "," << _Position[2] << "}, r = {0,0," << _Rotation << "}, s = {" << _Scale[0] << "," << _Scale[1] << "," << _Scale[2] << "} }";
    
    return true;
}
