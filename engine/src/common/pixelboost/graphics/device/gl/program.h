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
    
    std::string GetShaderLanguage() const;
    
    virtual bool SetSource(const std::string& source);
    
public:
    virtual void SetUniform(const std::string& name, int value);
    virtual void SetUniform(const std::string& name, float value);
    virtual void SetUniform(const std::string& name, const glm::vec3& value);
    virtual void SetUniform(const std::string& name, const glm::vec4& value);
    virtual void SetUniform(const std::string& name, const glm::mat4x4& value);
    
    virtual void OnContextLost();
    
private:
    GLuint GetUniformLocation(const std::string& name);
    
    bool CompileShader(GLenum type, GLuint* shader, const std::string& source);
    bool Link();
    
private:
#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    std::string _Source;
#endif
    std::map<std::string, GLuint> _Uniforms;
    
    GLuint _Program;
    GLuint _FragmentShader;
    GLuint _VertexShader;
    
    friend class GraphicsDeviceGL;
};
    
}

#endif
