#pragma once

#include <map>

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL

#include "pixelboost/graphics/device/gl/device.h"

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
    
    virtual void OnContextLost();
    
private:
    GLuint GetUniformLocation(const std::string& name);
    
    bool CompileShader(GLenum type, GLuint* shader, const std::string& source);
    
private:
    std::string _FragmentSource;
    std::string _VertexSource;
    std::map<int, std::string> _Attributes;
    std::map<std::string, GLuint> _Uniforms;
    
    GLuint _Program;
    GLuint _FragmentShader;
    GLuint _VertexShader;
    
    friend class GraphicsDeviceGL;
};
    
}

#endif
