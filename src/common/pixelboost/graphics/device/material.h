#pragma once

#include <string>

#include "glm/glm.hpp"
#include "sigslot/delegate.h"

#include "pixelboost/data/json/reader.h"

namespace pb
{
    
enum ShaderAttribute
{
    kShaderAttributeVertexPosition = 0,
    kShaderAttributeVertexNormal,
    kShaderAttributeVertexColor,
    kShaderAttributeVertexTexture0,
};
    
class Material
{
public:
    Material();
    virtual ~Material();
    
    bool Load(const std::string& filename);
    
    virtual void BindAttribute(int index, const std::string& name) = 0;
    
    virtual void SetUniform(const std::string& name, float value) = 0;
    virtual void SetUniform(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetUniform(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetUniform(const std::string& name, const glm::mat4x4& value) = 0;
    
protected:
    virtual void Load(json::Object& object) = 0;
    
    virtual void Bind() = 0;
};

}

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2
#include "pixelboost/graphics/device/gles2/material.h"
#endif
