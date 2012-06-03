#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/material.h"

#ifndef PIXELBOOST_DISABLE_GRAPHICS

using namespace pb;

Material::Material()
{
    
}

Material::~Material()
{
    
}

bool Material::Load(const std::string& filename)
{
    std::string material = FileHelpers::FileToString(pb::FileHelpers::GetRootPath() + filename);
    
    json::Object object;
    if (!json::Reader::Read(object, material))
        return false;
    
    Load(object);
    
    return true;
}

#endif
