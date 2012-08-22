#pragma once

#include <string>

#include "glm/glm.hpp"
#include "sigslot/delegate.h"

#include "pixelboost/data/json/reader.h"
#include "pixelboost/graphics/device/device.h"

namespace pugi
{
    class xml_node;
}

namespace pb
{
    
class Texture;
    
enum ShaderAttribute
{
    kShaderAttributeVertexPosition = 0,
    kShaderAttributeVertexWeight,
    kShaderAttributeVertexNormal,
    kShaderAttributeVertexColor0,
    kShaderAttributeVertexColor1,
    kShaderAttributeVertexFog,
    kShaderAttributeVertexPointSize,
    kShaderAttributeVertexBlend,
    kShaderAttributeVertexTexture0,
    kShaderAttributeVertexTexture1,
    kShaderAttributeVertexTexture2,
    kShaderAttributeVertexTexture3,
    kShaderAttributeVertexTexture4,
    kShaderAttributeVertexTexture5,
    kShaderAttributeVertexTexture6,
    kShaderAttributeVertexTexture7,
    kShaderAttributeVertexTangent = 14,
    kShaderAttributeVertexBinormal,
    kShaderAttributeCount,
};

class ShaderProgram
{
public:
    ShaderProgram();
    virtual ~ShaderProgram();
    
    virtual bool Load(const pugi::xml_node& attributes, const pugi::xml_node& pass) = 0;
    virtual bool Link() = 0;
    
    virtual void BindAttribute(int index, const std::string& name) = 0;
    
    virtual void SetUniform(const std::string& name, int value) = 0;
    virtual void SetUniform(const std::string& name, float value) = 0;
    virtual void SetUniform(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetUniform(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetUniform(const std::string& name, const glm::mat4x4& value) = 0;
    
    virtual void OnContextLost();
};
    
}

#ifdef PIXELBOOST_GRAPHICS_OPENGL
#include "pixelboost/graphics/device/gl/program.h"
#endif
