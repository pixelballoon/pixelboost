#pragma once

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/material.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2

namespace pb
{

class GraphicsDeviceGLES2;

class MaterialGLES2 : public Material
{
protected:
    MaterialGLES2(GraphicsDeviceGLES2* device);
    virtual ~MaterialGLES2();
    
public:
    virtual void BindAttribute(int index, const std::string& name);
    virtual void SetUniform(const std::string& name, float value);
    virtual void SetUniform(const std::string& name, const glm::vec3& value);
    virtual void SetUniform(const std::string& name, const glm::vec4& value);
    virtual void SetUniform(const std::string& name, const glm::mat4x4& value);
    
private:
    virtual bool CompileShader(GLenum type, GLuint* shader, const std::string& source);
    virtual bool Link();
    
protected:
    virtual void Load(json::Object& object);
    virtual void Bind();
    
private:
    GLuint _Program;
    
    friend class GraphicsDeviceGLES2;
};
    
}

#endif
