#include "glm/gtc/type_ptr.hpp"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/gles2/program.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2

using namespace pb;

ShaderProgramGLES2::ShaderProgramGLES2(GraphicsDeviceGLES2* device)
{
    _Program = glCreateProgram();
}

ShaderProgramGLES2::~ShaderProgramGLES2()
{
    glDeleteProgram(_Program);
}

bool ShaderProgramGLES2::Load(const std::string& fragmentSource, const std::string& vertexSource)
{
    if (!CompileShader(GL_FRAGMENT_SHADER, &_FragmentShader, fragmentSource))
        return false;
    
    if (!CompileShader(GL_VERTEX_SHADER, &_VertexShader, vertexSource))
        return false;
    
    glAttachShader(_Program, _FragmentShader);
    glAttachShader(_Program, _VertexShader);
    
    return true;
}

bool ShaderProgramGLES2::Link()
{
    glLinkProgram(_Program);
    
#ifndef PIXELBOOST_DISABLE_DEBUG
    GLint logLength;
    glGetProgramiv(_Program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* log = new GLchar[logLength];
        glGetProgramInfoLog(_Program, logLength, &logLength, log);
        printf("%s\n", log);
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

void ShaderProgramGLES2::BindAttribute(int index, const std::string& name)
{
    glBindAttribLocation(_Program, index, name.c_str());
}

void ShaderProgramGLES2::SetUniform(const std::string& name, float value)
{
    GLuint uniform = glGetUniformLocation(_Program, name.c_str());
    glUniform1f(uniform, value);
}

void ShaderProgramGLES2::SetUniform(const std::string& name, const glm::vec3& value)
{
    GLuint uniform = glGetUniformLocation(_Program, name.c_str());
    glUniform3f(uniform, value.x, value.y, value.z);
}

void ShaderProgramGLES2::SetUniform(const std::string& name, const glm::vec4& value)
{
    GLuint uniform = glGetUniformLocation(_Program, name.c_str());
    glUniform4f(uniform, value.x, value.y, value.z, value.w);
}

void ShaderProgramGLES2::SetUniform(const std::string& name, const glm::mat4x4& value)
{
    GLuint uniform = glGetUniformLocation(_Program, name.c_str());
    glUniformMatrix4fv(uniform, 1, 0, glm::value_ptr(value));
}

bool ShaderProgramGLES2::CompileShader(GLenum type, GLuint* shader, const std::string& source)
{
    *shader = glCreateShader(type);
    const GLchar* shaderSource = source.c_str();
    glShaderSource(*shader, 1, &shaderSource, NULL);
    glCompileShader(*shader);

#ifndef PIXELBOOST_DISABLE_DEBUG
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength)
    {
        GLchar* log = new GLchar[logLength];
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        printf("%s\n", log);
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

#endif
