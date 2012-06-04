#include "pixelboost/data/json/reader.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/material.h"
#include "pixelboost/graphics/device/program.h"

#ifndef PIXELBOOST_DISABLE_GRAPHICS

using namespace pb;

Material::Material()
{
    _Program = GraphicsDevice::Instance()->CreateProgram();
}

Material::~Material()
{
    GraphicsDevice::Instance()->DestroyProgram(_Program);
}

bool Material::Load(const std::string& filename)
{
    std::string material = FileHelpers::FileToString(pb::FileHelpers::GetRootPath() + filename);
    
    json::Object object;
    if (!json::Reader::Read(object, material))
        return false;
    
    std::string fragmentSource = FileHelpers::FileToString(FileHelpers::GetRootPath() + "/" + ((json::String)object["fragment"]).Value());
    std::string vertexSource = FileHelpers::FileToString(FileHelpers::GetRootPath() + "/" + ((json::String)object["vertex"]).Value());
    
    if (!_Program->Load(fragmentSource, vertexSource))
        return false;
    
    _Program->BindAttribute(kShaderAttributeVertexPosition, "position");
    
    if (!_Program->Link())
        return false;
    
    return true;
}

void Material::Bind()
{
    GraphicsDevice::Instance()->BindProgram(_Program);
}

ShaderProgram* Material::GetShaderProgram()
{
    return _Program;
}

#endif
