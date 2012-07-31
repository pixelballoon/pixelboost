#include "glm/gtc/type_ptr.hpp"

#include "pixelboost/debug/assert.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"

using namespace pb;

#ifdef PIXELBOOST_GRAPHICS_OPENGL

DeviceState::DeviceState()
{
    Reset();
}

void DeviceState::Reset()
{
    boundIndexBuffer = 0;
    boundTexture = 0;
    boundVertexBuffer = 0;
    boundProgram = 0;
    
    for (int i=0; i<kShaderAttributeCount; i++)
    {
        shaderAttribute[i] = false;
    }
}

GraphicsDevice* GraphicsDevice::Create()
{
    return new GraphicsDeviceGL();
}

GraphicsDeviceGL::GraphicsDeviceGL()
{
    
}

GraphicsDeviceGL::~GraphicsDeviceGL()
{
    
}

unsigned char* GraphicsDeviceGL::CaptureRenderBuffer()
{    
    int width, height;
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    unsigned char* buffer = new unsigned char[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    
    return buffer;
}

void GraphicsDeviceGL::ClearBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

VertexBuffer* GraphicsDeviceGL::CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
{
    VertexBuffer* vertexBuffer = GraphicsDevice::CreateVertexBuffer(bufferFormat, vertexFormat, length);
    
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    
    _VertexBuffers[vertexBuffer] = buffer;
    _VertexReverseBuffers[buffer] = vertexBuffer;
    
    return vertexBuffer;
}

void GraphicsDeviceGL::DestroyVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint buffer = _VertexBuffers[vertexBuffer];
    glDeleteBuffers(1, &buffer);
    
    _VertexBuffers.erase(vertexBuffer);
    _VertexReverseBuffers.erase(buffer);
    
    GraphicsDevice::DestroyVertexBuffer(vertexBuffer);
}

VertexBuffer* GraphicsDeviceGL::GetBoundVertexBuffer()
{
    VertexReverseMap::iterator it = _VertexReverseBuffers.find(_CurrentState.boundVertexBuffer);
    
    if (it != _VertexReverseBuffers.end())
        return it->second;
    
    return 0;
}

VertexBuffer* GraphicsDeviceGL::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint vertexBufferId = vertexBuffer ? _VertexBuffers[vertexBuffer] : 0;
    
    int previousBinding = _CurrentState.boundVertexBuffer;
    
    VertexBuffer* previousBuffer = 0;
    
    VertexReverseMap::iterator it = _VertexReverseBuffers.find(previousBinding);
    
    if (it != _VertexReverseBuffers.end())
        previousBuffer = it->second;
    
    if (vertexBufferId != _CurrentState.boundVertexBuffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        
        _CurrentState.boundVertexBuffer = vertexBufferId;
        
        if (vertexBuffer)
        {
            bool desiredShaderAttribute[kShaderAttributeCount];
            for (int i=0; i<kShaderAttributeCount; i++)
            {
                desiredShaderAttribute[i] = false;;
            }
            
            switch (vertexBuffer->GetVertexFormat())
            {
                case kVertexFormat_P_XY_RGBA:
                {
                    GLsizei stride = sizeof(Vertex_PXY_RGBA);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexColor] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXY_RGBA, position));
                    glVertexAttribPointer(kShaderAttributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXY_RGBA, color));
                    break;
                }
                case kVertexFormat_P_XYZ:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ, position));
                    break;
                }
                case kVertexFormat_P_XYZ_UV:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_UV);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexTexture0] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_UV, uv));
                    break;
                }
                case kVertexFormat_P_XYZ_RGBA:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_RGBA);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexColor] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA, position));
                    glVertexAttribPointer(kShaderAttributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA, color));
                    break;
                }
                case kVertexFormat_P_XYZ_RGBA_UV:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_RGBA_UV);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexColor] = true;
                    desiredShaderAttribute[kShaderAttributeVertexTexture0] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, color));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, uv));
                    break;
                }
                case kVertexFormat_NP_XYZ_UV:
                {
                    GLsizei stride = sizeof(Vertex_NPXYZ_UV);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexNormal] = true;
                    desiredShaderAttribute[kShaderAttributeVertexTexture0] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_NPXYZ_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexNormal, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_NPXYZ_UV, normal));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_NPXYZ_UV, uv));
                    break;
                }
            }
            
            for (int i=0; i<kShaderAttributeCount; i++)
            {
                if (desiredShaderAttribute[i] != _CurrentState.shaderAttribute[i])
                {
                    if (desiredShaderAttribute[i])
                        glEnableVertexAttribArray(i);
                    else
                        glDisableVertexAttribArray(i);
                    
                    _CurrentState.shaderAttribute[i] = desiredShaderAttribute[i];
                }
            }
        }
    }
    
    return previousBuffer;
}

void GraphicsDeviceGL::LockVertexBuffer(VertexBuffer* vertexBuffer)
{
    GraphicsDevice::LockVertexBuffer(vertexBuffer);
}

void GraphicsDeviceGL::UnlockVertexBuffer(VertexBuffer* vertexBuffer, int numElements)
{
    if (numElements == 0)
        return;

    if (numElements == -1)
        numElements = vertexBuffer->GetMaxSize();
    
    GLenum bufferType = vertexBuffer->GetBufferFormat() == kBufferFormatStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
    
    VertexBuffer* previousBuffer = BindVertexBuffer(vertexBuffer);
    switch (vertexBuffer->GetVertexFormat())
    {
        case kVertexFormat_P_XY_RGBA:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_PXY_RGBA) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P_XYZ:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_PXYZ) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P_XYZ_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_PXYZ_UV) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P_XYZ_RGBA:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_PXYZ_RGBA) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P_XYZ_RGBA_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_PXYZ_RGBA_UV) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_NP_XYZ_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_NPXYZ_UV) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
    }
    BindVertexBuffer(previousBuffer);
}
    
IndexBuffer* GraphicsDeviceGL::CreateIndexBuffer(BufferFormat bufferFormat, int length)
{
    IndexBuffer* indexBuffer = GraphicsDevice::CreateIndexBuffer(bufferFormat, length);
    
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    
    _IndexBuffers[indexBuffer] = buffer;
    _IndexReverseBuffers[buffer] = indexBuffer;
    
    return indexBuffer;
}

void GraphicsDeviceGL::DestroyIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint buffer = _IndexBuffers[indexBuffer];
    glDeleteBuffers(1, &buffer);
    
    _IndexBuffers.erase(indexBuffer);
    _IndexReverseBuffers.erase(buffer);
    
    GraphicsDevice::DestroyIndexBuffer(indexBuffer);
}

IndexBuffer* GraphicsDeviceGL::GetBoundIndexBuffer()
{
    IndexReverseMap::iterator it = _IndexReverseBuffers.find(_CurrentState.boundIndexBuffer);
    
    if (it != _IndexReverseBuffers.end())
        return it->second;
    
    return 0;
}

IndexBuffer* GraphicsDeviceGL::BindIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint indexBufferId = indexBuffer ? _IndexBuffers[indexBuffer] : 0;
    
    IndexBuffer* previousBuffer = GetBoundIndexBuffer();
    
    if (indexBufferId != _CurrentState.boundIndexBuffer)
    {
        _CurrentState.boundIndexBuffer = indexBufferId;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    }
    
    return previousBuffer;
}

void GraphicsDeviceGL::LockIndexBuffer(IndexBuffer* indexBuffer)
{
    GraphicsDevice::LockIndexBuffer(indexBuffer);
}

void GraphicsDeviceGL::UnlockIndexBuffer(IndexBuffer* indexBuffer, int numElements)
{
    if (numElements == 0)
        return;
        
    if (numElements == -1)
        numElements = indexBuffer->GetLength();
    
    IndexBuffer* previousBuffer = BindIndexBuffer(indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numElements, indexBuffer->GetData(), indexBuffer->GetBufferFormat() == kBufferFormatStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    BindIndexBuffer(previousBuffer);
}
    
Texture* GraphicsDeviceGL::CreateTexture()
{
    TextureGL* texture = new TextureGL(this);
    _Textures.push_back(texture);
    return texture;
}

void GraphicsDeviceGL::DestroyTexture(Texture* texture)
{
    for (TextureList::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        if (*it == texture)
        {
            _Textures.erase(it);
            GraphicsDevice::DestroyTexture(texture);
            return;
        }
    }
}

Texture* GraphicsDeviceGL::GetBoundTexture()
{
    for (TextureList::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        if (static_cast<TextureGL*>(*it)->_Texture == _CurrentState.boundTexture)
            return *it;
    }
    
    return 0;
}
    
Texture* GraphicsDeviceGL::BindTexture(Texture* texture)
{
    GLuint textureId = texture ? static_cast<TextureGL*>(texture)->_Texture : 0;
    
    Texture* previousTexture = GetBoundTexture();
    
    if (textureId != _CurrentState.boundTexture)
    {
        _CurrentState.boundTexture = textureId;
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
    
    return previousTexture;
}

ShaderProgram* GraphicsDeviceGL::CreateProgram()
{
    ShaderProgramGL* program = new ShaderProgramGL(this);
    _Programs.push_back(program);
    return program;
}

void GraphicsDeviceGL::DestroyProgram(ShaderProgram* program)
{
    for (ProgramList::iterator it = _Programs.begin(); it != _Programs.end(); ++it)
    {
        if (*it == program)
        {
            _Programs.erase(it);
            GraphicsDevice::DestroyProgram(program);
            return;
        }
    }
}

ShaderProgram* GraphicsDeviceGL::GetBoundProgram()
{
    for (ProgramList::iterator it = _Programs.begin(); it != _Programs.end(); ++it)
    {
        if (static_cast<ShaderProgramGL*>(*it)->_Program == _CurrentState.boundProgram)
            return *it;
    }
    
    return 0;
}

ShaderProgram* GraphicsDeviceGL::BindProgram(ShaderProgram* program)
{
    GLuint programId = program ? static_cast<ShaderProgramGL*>(program)->_Program : 0;
    ShaderProgramGL* previousProgram = static_cast<ShaderProgramGL*>(GetBoundProgram());
    
    if (static_cast<ShaderProgramGL*>(program)->_Program != _CurrentState.boundProgram)
    {
        glUseProgram(programId);
        _CurrentState.boundProgram = programId;
    }
    
    return previousProgram;
}

void GraphicsDeviceGL::OnContextLost()
{
#ifdef PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    _CurrentState.Reset();
    
    _IndexReverseBuffers.clear();
    _VertexReverseBuffers.clear();
    
    for (IndexMap::iterator it = _IndexBuffers.begin(); it != _IndexBuffers.end(); ++it)
    {
        GLuint buffer = 0;
        glGenBuffers(1, &buffer);
        
        _IndexBuffers[it->first] = buffer;
        _IndexReverseBuffers[buffer] = it->first;
        LockIndexBuffer(it->first);
        UnlockIndexBuffer(it->first);
    }
    
    for (VertexMap::iterator it = _VertexBuffers.begin(); it != _VertexBuffers.end(); ++it)
    {
        GLuint buffer = 0;
        glGenBuffers(1, &buffer);
        
        _VertexBuffers[it->first] = buffer;
        _VertexReverseBuffers[buffer] = it->first;
        LockVertexBuffer(it->first);
        UnlockVertexBuffer(it->first, it->first->GetCurrentSize());
    }
    
    for (TextureList::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        (*it)->OnContextLost();
    }
    
    for (ProgramList::iterator it = _Programs.begin(); it != _Programs.end(); ++it)
    {
        (*it)->OnContextLost();
    }
#else
    PbAssert(!"Context loss has not been enabled");
#endif
}

void GraphicsDeviceGL::SetState(State state, bool enable)
{
    GLenum glState;
    
    switch (state)
    {
        case kStateBlend:
            glState = GL_BLEND;
            break;
        case kStateDepthTest:
            glState = GL_DEPTH_TEST;
            break;
        case kStateTexture2D:
            glState = GL_TEXTURE_2D;
            break;
        case kStateScissor:
            glState = GL_SCISSOR_TEST;
            break;
    }
    
    if (enable)
    {
        glEnable(glState);
    } else {
        glDisable(glState);
    }
}

void GraphicsDeviceGL::SetBlendMode(Blend source, Blend destination)
{
    GLenum glSource;
    GLenum glDest;
    
    switch (source)
    {
        case kBlendOne:
            glSource = GL_ONE;
            break;
        case kBlendOneMinusSourceAlpha:
            glSource = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case kBlendSourceAlpha:
            glSource = GL_SRC_ALPHA;
            break;
    }
    
    switch (destination)
    {
        case kBlendOne:
            glDest = GL_ONE;
            break;
        case kBlendOneMinusSourceAlpha:
            glDest = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case kBlendSourceAlpha:
            glDest = GL_SRC_ALPHA;
            break;
    }
    
    glBlendFunc(glSource, glDest);
}

void GraphicsDeviceGL::SetViewport(glm::vec4 viewport)
{
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}

void GraphicsDeviceGL::SetScissor(glm::vec4 scissor)
{
    glScissor(scissor.x, scissor.y, scissor.z, scissor.w);
}

void GraphicsDeviceGL::DrawElements(ElementType elementType, int num)
{
    GLuint type;
    
    switch (elementType)
    {
        case kElementLines:
        {
            type = GL_LINES;
            break;
        }
        case kElementLineLoop:
        {
            type = GL_LINE_LOOP;
            break;
        }
        case kElementTriangles:
        {
            type = GL_TRIANGLES;
            break;
        }
        case kElementTriangleFan:
        {
            type = GL_TRIANGLE_FAN;
            break;
        }
    }
    
    glDrawElements(type, num, GL_UNSIGNED_SHORT, (void*)0);
}

#endif
