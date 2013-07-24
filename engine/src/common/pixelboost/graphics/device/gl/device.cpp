#include "glm/gtc/type_ptr.hpp"

#include "pixelboost/debug/assert.h"
#include "pixelboost/graphics/device/gl/device.h"
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
    boundVertexBuffer = 0;
    boundProgram = 0;
    
    for (int i=0; i<kNumTextureUnits; i++)
    {
        boundTexture[i] = 0;
    }
    
    for (int i=0; i<kShaderAttributeCount; i++)
    {
        shaderAttribute[i] = false;
    }
    
    states.clear();
}

void DeviceState::UpdateAll(DeviceState& state)
{
    UpdateTexture(state);
    UpdateIndexBuffer(state);
    UpdateVertexBuffer(state);
    
    UpdateStates(state);
}
 
void DeviceState::UpdateIndexBuffer(DeviceState& state)
{
    if (boundIndexBuffer != state.boundIndexBuffer)
    {
        boundIndexBuffer = state.boundIndexBuffer;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundIndexBuffer);
    }
}

void DeviceState::UpdateVertexBuffer(DeviceState& state)
{
    GraphicsDeviceGL* device = static_cast<GraphicsDeviceGL*>(GraphicsDevice::Instance());

    if (boundVertexBuffer != state.boundVertexBuffer)
    {
        boundVertexBuffer = state.boundVertexBuffer;
        glBindBuffer(GL_ARRAY_BUFFER, boundVertexBuffer);
        
        VertexBuffer* vertexBuffer = 0;
        
        GraphicsDeviceGL::VertexReverseMap::iterator vertexBufferIt = device->_VertexReverseBuffers.find(boundVertexBuffer);
        
        if (vertexBufferIt != device->_VertexReverseBuffers.end())
            vertexBuffer = vertexBufferIt->second;
        
        if (vertexBuffer)
        {
            bool desiredShaderAttribute[kShaderAttributeCount];
            for (int i=0; i<kShaderAttributeCount; i++)
            {
                desiredShaderAttribute[i] = false;
            }
            
            switch (vertexBuffer->GetVertexFormat())
            {
                case kVertexFormat_P3:
                {
                    GLsizei stride = sizeof(Vertex_P3);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3, position));
                    break;
                }
                case kVertexFormat_P3_UV:
                {
                    GLsizei stride = sizeof(Vertex_P3_UV);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexTexture0] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_UV, uv));
                    break;
                }
                case kVertexFormat_P3_C4:
                {
                    GLsizei stride = sizeof(Vertex_P3_C4);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexColor0] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_C4, position));
                    glVertexAttribPointer(kShaderAttributeVertexColor0, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_C4, color));
                    break;
                }
                case kVertexFormat_P3_C4_UV:
                {
                    GLsizei stride = sizeof(Vertex_P3_C4_UV);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexColor0] = true;
                    desiredShaderAttribute[kShaderAttributeVertexTexture0] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_C4_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexColor0, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_C4_UV, color));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_C4_UV, uv));
                    break;
                }
                case kVertexFormat_P3_N3_UV:
                {
                    GLsizei stride = sizeof(Vertex_P3_N3_UV);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexNormal] = true;
                    desiredShaderAttribute[kShaderAttributeVertexTexture0] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_N3_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexNormal, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_N3_UV, normal));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_N3_UV, uv));
                    break;
                }
                case kVertexFormat_P3_N3_UV_BW:
                {
                    GLsizei stride = sizeof(Vertex_P3_N3_UV_BW);
                    desiredShaderAttribute[kShaderAttributeVertexPosition] = true;
                    desiredShaderAttribute[kShaderAttributeVertexNormal] = true;
                    desiredShaderAttribute[kShaderAttributeVertexTexture0] = true;
                    desiredShaderAttribute[kShaderAttributeVertexWeight] = true;
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_N3_UV_BW, position));
                    glVertexAttribPointer(kShaderAttributeVertexNormal, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_N3_UV_BW, normal));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_P3_N3_UV_BW, uv));
                    break;
                }
            }
            
            for (int i=0; i<kShaderAttributeCount; i++)
            {
                if (desiredShaderAttribute[i] != shaderAttribute[i])
                {
                    if (desiredShaderAttribute[i])
                        glEnableVertexAttribArray(i);
                    else
                        glDisableVertexAttribArray(i);
                    
                    shaderAttribute[i] = desiredShaderAttribute[i];
                }
            }
        }
    }
}

void DeviceState::UpdateTexture(DeviceState& state)
{
    for (int i=0; i<kNumTextureUnits; i++)
    {
        if (boundTexture[i] != state.boundTexture[i])
        {
            boundTexture[i] = state.boundTexture[i];
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D, boundTexture[i]);
        }
    }
}

void DeviceState::UpdateStates(DeviceState& state)
{
    for (StateMap::iterator it = state.states.begin(); it != state.states.end(); ++it)
    {
        StateMap::iterator currentState = states.find(it->first);
        
        if (currentState == states.end() || currentState->second != it->second)
        {
            states[it->first] = it->second;
            
            if (it->second)
            {
                glEnable(it->first);
            } else {
                glDisable(it->first);
            }
        }
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

std::shared_ptr<RenderBufferCapture> GraphicsDeviceGL::CaptureRenderBuffer()
{    
    GLint width = GetDisplayResolution().x;
    GLint height = GetDisplayResolution().y;

    unsigned char* buffer = new unsigned char[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    
    return std::shared_ptr<RenderBufferCapture>(new RenderBufferCapture(buffer, width, height));
}

void GraphicsDeviceGL::SetClearColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void GraphicsDeviceGL::ClearBuffers()
{
    _CurrentState.UpdateAll(_DesiredState);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void GraphicsDeviceGL::ResetState()
{
    _DesiredState.Reset();
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
    
    if (_CurrentState.boundVertexBuffer == buffer)
    {
        _CurrentState.boundVertexBuffer = 0;
    }
    
    _VertexBuffers.erase(vertexBuffer);
    _VertexReverseBuffers.erase(buffer);
    
    GraphicsDevice::DestroyVertexBuffer(vertexBuffer);
}

VertexBuffer* GraphicsDeviceGL::GetBoundVertexBuffer()
{
    VertexReverseMap::iterator it = _VertexReverseBuffers.find(_DesiredState.boundVertexBuffer);
    
    if (it != _VertexReverseBuffers.end())
        return it->second;
    
    return 0;
}

VertexBuffer* GraphicsDeviceGL::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint vertexBufferId = vertexBuffer ? _VertexBuffers[vertexBuffer] : 0;
    
    int previousBinding = _DesiredState.boundVertexBuffer;
    
    VertexBuffer* previousBuffer = 0;
    
    VertexReverseMap::iterator it = _VertexReverseBuffers.find(previousBinding);
    
    if (it != _VertexReverseBuffers.end())
        previousBuffer = it->second;
    
    _DesiredState.boundVertexBuffer = vertexBufferId;
    
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
    
    _CurrentState.UpdateVertexBuffer(_DesiredState);
    
    switch (vertexBuffer->GetVertexFormat())
    {
        case kVertexFormat_P3:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_P3) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P3_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_P3_UV) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P3_C4:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_P3_C4) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P3_C4_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_P3_C4_UV) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P3_N3_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_P3_N3_UV) * numElements, vertexBuffer->GetData(), bufferType);
            break;
        }
        case kVertexFormat_P3_N3_UV_BW:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(kVertexFormat_P3_N3_UV_BW) * numElements, vertexBuffer->GetData(), bufferType);
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
    
    if (_CurrentState.boundIndexBuffer == buffer)
    {
        _CurrentState.boundIndexBuffer = 0;
    }
    
    _IndexBuffers.erase(indexBuffer);
    _IndexReverseBuffers.erase(buffer);
    
    GraphicsDevice::DestroyIndexBuffer(indexBuffer);
}

IndexBuffer* GraphicsDeviceGL::GetBoundIndexBuffer()
{
    IndexReverseMap::iterator it = _IndexReverseBuffers.find(_DesiredState.boundIndexBuffer);
    
    if (it != _IndexReverseBuffers.end())
        return it->second;
    
    return 0;
}

IndexBuffer* GraphicsDeviceGL::BindIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint indexBufferId = indexBuffer ? _IndexBuffers[indexBuffer] : 0;
    
    IndexBuffer* previousBuffer = GetBoundIndexBuffer();
    
    _DesiredState.boundIndexBuffer = indexBufferId;
    
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
        numElements = indexBuffer->GetMaxSize();
    
    IndexBuffer* previousBuffer = BindIndexBuffer(indexBuffer);
    
    _CurrentState.UpdateIndexBuffer(_DesiredState);
    
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

Texture* GraphicsDeviceGL::GetBoundTexture(int textureUnit)
{
    for (TextureList::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        if (static_cast<TextureGL*>(*it)->_Texture == _DesiredState.boundTexture[textureUnit])
            return *it;
    }
    
    return 0;
}
    
Texture* GraphicsDeviceGL::BindTexture(int textureUnit, Texture* texture, bool force)
{
    GLuint textureId = texture ? static_cast<TextureGL*>(texture)->_Texture : 0;
    
    Texture* previousTexture = GetBoundTexture(textureUnit);
    
    _DesiredState.boundTexture[textureUnit] = textureId;
    
    if (force)
        _CurrentState.UpdateTexture(_DesiredState);
        
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
    _DesiredState.Reset();
    
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
        case kStateScissor:
            glState = GL_SCISSOR_TEST;
            break;
    }
    
    _DesiredState.states[glState] = enable;
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
    if (num == 0)
        return;
    
    _CurrentState.UpdateAll(_DesiredState);
    
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
