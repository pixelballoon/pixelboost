#include "glm/gtc/type_ptr.hpp"

#include "pugixml/pugixml.hpp"

#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/gl/program.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL

using namespace pb;

ShaderProgramGL::ShaderProgramGL(GraphicsDeviceGL* device)
{
    
}

ShaderProgramGL::~ShaderProgramGL()
{
    glDeleteProgram(_Program);
}

bool ShaderProgramGL::Load(const pugi::xml_node& attributes, const pugi::xml_node& pass)
{
    std::string programType;
    
    pugi::xml_node program = pass.find_child_by_attribute("program", "language", "glsl");
    
    if (program.empty())
        return false;

    _Uniforms.clear();
    
#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    _Source = program.child_value();
#endif
    
    return CreateShader(program.child_value());
}

bool ShaderProgramGL::Link()
{
    glLinkProgram(_Program);
    
    if (glGetAttribLocation(_Program, "PB_Attr_Position") >= 0)
        glBindAttribLocation(_Program, kShaderAttributeVertexPosition, "PB_Attr_Position");
    
    if (glGetAttribLocation(_Program, "PB_Attr_Normal") >= 0)
        glBindAttribLocation(_Program, kShaderAttributeVertexNormal, "PB_Attr_Normal");
    
    if (glGetAttribLocation(_Program, "PB_Attr_Color0") >= 0)
        glBindAttribLocation(_Program, kShaderAttributeVertexColor0, "PB_Attr_Color0");
    
    if (glGetAttribLocation(_Program, "PB_Attr_Texture0") >= 0)
        glBindAttribLocation(_Program, kShaderAttributeVertexTexture0, "PB_Attr_Texture0");

    glLinkProgram(_Program);
    
#ifndef PIXELBOOST_DISABLE_DEBUG
    GLint logLength;
    glGetProgramiv(_Program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* log = new GLchar[logLength];
        glGetProgramInfoLog(_Program, logLength, &logLength, log);
        PbLogError("pb.graphics.shader", "Error linking shader (%s)", log);
        delete log;
    }
#endif
    
    GLint status;
    glGetProgramiv(_Program, GL_LINK_STATUS, &status);
    if (!status)
    {
        glDeleteShader(_FragmentShader);
        glDeleteShader(_VertexShader);
        glDeleteProgram(_Program);
        _FragmentShader = 0;
        _VertexShader = 0;
        _Program = 0;
        return false;
    }
    
    return true;
}

void ShaderProgramGL::SetUniform(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgramGL::SetUniform(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgramGL::SetUniform(const std::string& name, const glm::vec3& value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void ShaderProgramGL::SetUniform(const std::string& name, const glm::vec4& value)
{
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void ShaderProgramGL::SetUniform(const std::string& name, const glm::mat4x4& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, 0, glm::value_ptr(value));
}

GLuint ShaderProgramGL::GetUniformLocation(const std::string& name)
{
    std::map<std::string, GLuint>::iterator it = _Uniforms.find(name);
    
    if (it != _Uniforms.end())
        return it->second;
 
    GLuint location = glGetUniformLocation(_Program, name.c_str());
    _Uniforms[name] = location;
    return location;
}

bool ShaderProgramGL::CreateShader(const std::string& source)
{
    _Program = glCreateProgram();
    
    if (!CompileShader(GL_FRAGMENT_SHADER, &_FragmentShader, source))
        return false;
    
    if (!CompileShader(GL_VERTEX_SHADER, &_VertexShader, source))
        return false;
    
    glAttachShader(_Program, _FragmentShader);
    glAttachShader(_Program, _VertexShader);
    
    return true;
}

bool ShaderProgramGL::CompileShader(GLenum type, GLuint* shader, const std::string& source)
{
    *shader = glCreateShader(type);
#ifdef PIXELBOOST_GRAPHICS_OPENGLES2
    const GLchar* shaderSource[3] = {"#version 100\n#define PLATFORM_GLES\n#define PLATFORM_GLSL1\n", type == GL_VERTEX_SHADER ? "#define VERTEX\n" : "#define FRAGMENT\n", source.c_str()};
#endif
#ifdef PIXELBOOST_GRAPHICS_OPENGL2
    const GLchar* shaderSource[3] = {"#version 120\n#define PLATFORM_GL\n#define PLATFORM_GLSL1\n#define lowp\n#define mediump\n#define highp\n", type == GL_VERTEX_SHADER ? "#define VERTEX\n" : "#define FRAGMENT\n", source.c_str()};
#endif
    glShaderSource(*shader, 3, shaderSource, NULL);
    glCompileShader(*shader);

#ifndef PIXELBOOST_DISABLE_DEBUG
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength)
    {
        GLchar* log = new GLchar[logLength];
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        PbLogError("pb.graphics.shader", "Error compiling shader (%s)", log);
        delete log;
    }
#endif
    
    GLint status;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
}

void ShaderProgramGL::OnContextLost()
{
#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    CreateShader(_Source);
    
    Link();
#else
    PbAssert(!"Context loss has not been enabled");
#endif
}

#endif
