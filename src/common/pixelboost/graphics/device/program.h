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

class ShaderProgram
{
public:
    ShaderProgram();
    virtual ~ShaderProgram();
    
    virtual bool Load(const std::string& vertexSource, const std::string& fragmentSource) = 0;
    virtual bool Link() = 0;
    
    virtual void BindAttribute(int index, const std::string& name) = 0;
    
    virtual void SetUniform(const std::string& name, float value) = 0;
    virtual void SetUniform(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetUniform(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetUniform(const std::string& name, const glm::mat4x4& value) = 0;
};
    
}

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2
#include "pixelboost/graphics/device/gles2/program.h"
#endif
