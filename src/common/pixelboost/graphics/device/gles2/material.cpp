#include "glm/gtc/type_ptr.hpp"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/gles2/material.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2

using namespace pb;

MaterialGLES2::MaterialGLES2(GraphicsDeviceGLES2* device)
{
    /*
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    */
    
    _Program = glCreateProgram();
}

MaterialGLES2::~MaterialGLES2()
{
    glDeleteProgram(_Program);
}

void MaterialGLES2::BindAttribute(int index, const std::string& name)
{
    glBindAttribLocation(_Program, index, name.c_str());
}

void MaterialGLES2::SetUniform(const std::string& name, float value)
{
    GLuint uniform = glGetUniformLocation(_Program, name.c_str());
    glUniform1f(uniform, value);
}

void MaterialGLES2::SetUniform(const std::string& name, const glm::vec3& value)
{
    GLuint uniform = glGetUniformLocation(_Program, name.c_str());
    glUniform3f(uniform, value.x, value.y, value.z);
}

void MaterialGLES2::SetUniform(const std::string& name, const glm::vec4& value)
{
    GLuint uniform = glGetUniformLocation(_Program, name.c_str());
    glUniform4f(uniform, value.x, value.y, value.z, value.w);
}

void MaterialGLES2::SetUniform(const std::string& name, const glm::mat4x4& value)
{
    GLuint uniform = glGetUniformLocation(_Program, name.c_str());
    glUniformMatrix4fv(uniform, 1, 0, glm::value_ptr(value));
}

bool MaterialGLES2::CompileShader(GLenum type, GLuint* shader, const std::string& source)
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

bool MaterialGLES2::Link()
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
        return false;
    
    return true;
}

void MaterialGLES2::Load(json::Object& object)
{
    _Program = glCreateProgram();
    
    std::string fragmentSource = FileHelpers::FileToString(FileHelpers::GetRootPath() + "/" + ((json::String)object["fragment"]).Value());
    std::string vertexSource = FileHelpers::FileToString(FileHelpers::GetRootPath() + "/" + ((json::String)object["vertex"]).Value());
    
    GLuint fragmentShader;
    GLuint vertexShader;
    
    CompileShader(GL_FRAGMENT_SHADER, &fragmentShader, fragmentSource);
    CompileShader(GL_VERTEX_SHADER, &vertexShader, vertexSource);
    
    glAttachShader(_Program, fragmentShader);
    glAttachShader(_Program, vertexShader);
    
    glBindAttribLocation(_Program, kShaderAttributeVertexPosition, "position");
    /*
    glBindAttribLocation(_Program, kShaderAttributeVertexNormal, "normal");
    glBindAttribLocation(_Program, kShaderAttributeVertexColor, "color");
    glBindAttribLocation(_Program, kShaderAttributeVertexTexture0, "texture_0");
    */
    
    if (!Link())
    {
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteProgram(_Program);
        _Program = 0;
        
        return;
    }
    
    if (vertexShader)
    {
        glDetachShader(_Program, vertexShader);
        glDeleteShader(vertexShader);
    }
    
    if (fragmentShader)
    {
        glDetachShader(_Program, fragmentShader);
        glDeleteShader(fragmentShader);
    }
    
    /*
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    
    // Create shader program.
    _program = glCreateProgram();
    
    // Create and compile vertex shader.
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }
    
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(_program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(_program, ATTRIB_NORMAL, "normal");
    
    // Link program.
    if (![self linkProgram:_program]) {
        NSLog(@"Failed to link program: %d", _program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_program) {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return NO;
    }
    
    // Get uniform locations.
    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(_program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
     */
}

void MaterialGLES2::Bind()
{
    glUseProgram(_Program);
}

#endif
