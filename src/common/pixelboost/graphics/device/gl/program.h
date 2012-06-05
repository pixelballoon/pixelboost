#pragma once

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL

namespace pb
{

class GraphicsDeviceGL;

class ShaderProgramGL : public ShaderProgram
{
protected:
    ShaderProgramGL(GraphicsDeviceGL* device);
    virtual ~ShaderProgramGL();
    
    virtual bool Load(const std::string& vertexSource, const std::string& fragmentSource);
    virtual bool Link();
    
public:
    virtual void BindAttribute(int index, const std::string& name);
    
    virtual void SetUniform(const std::string& name, int value);
    virtual void SetUniform(const std::string& name, float value);
    virtual void SetUniform(const std::string& name, const glm::vec3& value);
    virtual void SetUniform(const std::string& name, const glm::vec4& value);
    virtual void SetUniform(const std::string& name, const glm::mat4x4& value);
    
private:
    virtual bool CompileShader(GLenum type, GLuint* shader, const std::string& source);
    
private:
    GLuint _Program;
    GLuint _FragmentShader;
    GLuint _VertexShader;
    
    friend class GraphicsDeviceGL;
};
    
}

#endif
