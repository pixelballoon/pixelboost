#include "glm/gtc/type_ptr.hpp"

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/material.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"

using namespace pb;

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2

#include <OpenGLES/ES2/gl.h>

struct pb::DeviceState
{
    DeviceState()
    {
        boundIndexBuffer = 0;
        boundTexture = 0;
        boundVertexBuffer = 0;
        boundMaterial = 0;
    }
    
    GLuint boundIndexBuffer;
    GLuint boundTexture;
    GLuint boundVertexBuffer;
    Material* boundMaterial;
};

GraphicsDevice* GraphicsDevice::Create()
{
    return new GraphicsDeviceGLES2();
}

GraphicsDeviceGLES2::GraphicsDeviceGLES2()
{
    _State = new DeviceState();
}

GraphicsDeviceGLES2::~GraphicsDeviceGLES2()
{
    delete _State;
}

unsigned char* GraphicsDeviceGLES2::CaptureRenderBuffer()
{    
    int width, height;
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    unsigned char* buffer = new unsigned char[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    
    return buffer;
}

VertexBuffer* GraphicsDeviceGLES2::CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
{
    VertexBuffer* vertexBuffer = GraphicsDevice::CreateVertexBuffer(bufferFormat, vertexFormat, length);
    
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    
    _VertexBuffers[vertexBuffer] = buffer;
    _VertexReverseBuffers[buffer] = vertexBuffer;
    
    return vertexBuffer;
}

void GraphicsDeviceGLES2::DestroyVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint buffer = _VertexBuffers[vertexBuffer];
    glDeleteBuffers(1, &buffer);
    
    _VertexBuffers.erase(vertexBuffer);
    _VertexReverseBuffers.erase(buffer);
    
    GraphicsDevice::DestroyVertexBuffer(vertexBuffer);
}

VertexBuffer* GraphicsDeviceGLES2::GetBoundVertexBuffer()
{
    VertexReverseMap::iterator it = _VertexReverseBuffers.find(_State->boundVertexBuffer);
    
    if (it != _VertexReverseBuffers.end())
        return it->second;
    
    return 0;
}

VertexBuffer* GraphicsDeviceGLES2::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint vertexBufferId = vertexBuffer ? _VertexBuffers[vertexBuffer] : 0;
    
    int previousBinding = _State->boundVertexBuffer;
    
    VertexBuffer* previousBuffer = 0;
    
    VertexReverseMap::iterator it = _VertexReverseBuffers.find(previousBinding);
    
    if (it != _VertexReverseBuffers.end())
        previousBuffer = it->second;
    
    if (vertexBufferId != _State->boundVertexBuffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        
        _State->boundVertexBuffer = vertexBufferId;
        
        if (vertexBuffer)
        {
            switch (vertexBuffer->GetVertexFormat())
            {
                case kVertexFormat_P_XY_RGBA:
                {
                    GLsizei stride = sizeof(Vertex_PXY_RGBA);
                    glEnableVertexAttribArray(kShaderAttributeVertexPosition);
                    glEnableVertexAttribArray(kShaderAttributeVertexColor);
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXY_RGBA, position));
                    glVertexAttribPointer(kShaderAttributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXY_RGBA, color));
                    break;
                }
                case kVertexFormat_P_XYZ:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ);
                    glEnableVertexAttribArray(kShaderAttributeVertexPosition);
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ, position));
                    break;
                }
                case kVertexFormat_P_XYZ_UV:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_UV);
                    glEnableVertexAttribArray(kShaderAttributeVertexPosition);
                    glEnableVertexAttribArray(kShaderAttributeVertexTexture0);
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_UV, uv));
                    break;
                }
                case kVertexFormat_P_XYZ_RGBA:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_RGBA);
                    glEnableVertexAttribArray(kShaderAttributeVertexPosition);
                    glEnableVertexAttribArray(kShaderAttributeVertexColor);
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA, position));
                    glVertexAttribPointer(kShaderAttributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA, color));
                    break;
                }
                case kVertexFormat_P_XYZ_RGBA_UV:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_RGBA_UV);
                    glEnableVertexAttribArray(kShaderAttributeVertexPosition);
                    glEnableVertexAttribArray(kShaderAttributeVertexColor);
                    glEnableVertexAttribArray(kShaderAttributeVertexTexture0);
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, color));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, uv));
                    break;
                }
                case kVertexFormat_NP_XYZ_UV:
                {
                    GLsizei stride = sizeof(Vertex_NPXYZ_UV);
                    glEnableVertexAttribArray(kShaderAttributeVertexPosition);
                    glEnableVertexAttribArray(kShaderAttributeVertexNormal);
                    glEnableVertexAttribArray(kShaderAttributeVertexTexture0);
                    glVertexAttribPointer(kShaderAttributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_NPXYZ_UV, position));
                    glVertexAttribPointer(kShaderAttributeVertexNormal, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_NPXYZ_UV, normal));
                    glVertexAttribPointer(kShaderAttributeVertexTexture0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex_NPXYZ_UV, uv));
                    break;
                }
            }
        }
    }
    
    return previousBuffer;
}

void GraphicsDeviceGLES2::LockVertexBuffer(VertexBuffer* vertexBuffer)
{
    GraphicsDevice::LockVertexBuffer(vertexBuffer);
}

void GraphicsDeviceGLES2::UnlockVertexBuffer(VertexBuffer* vertexBuffer, int numElements)
{
    if (numElements == 0)
        return;

    if (numElements == -1)
        numElements = vertexBuffer->GetLength();
    
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
    
IndexBuffer* GraphicsDeviceGLES2::CreateIndexBuffer(BufferFormat bufferFormat, int length)
{
    IndexBuffer* indexBuffer = GraphicsDevice::CreateIndexBuffer(bufferFormat, length);
    
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    
    _IndexBuffers[indexBuffer] = buffer;
    _IndexReverseBuffers[buffer] = indexBuffer;
    
    return indexBuffer;
}

void GraphicsDeviceGLES2::DestroyIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint buffer = _IndexBuffers[indexBuffer];
    glDeleteBuffers(1, &buffer);
    
    _IndexBuffers.erase(indexBuffer);
    _IndexReverseBuffers.erase(buffer);
    
    GraphicsDevice::DestroyIndexBuffer(indexBuffer);
}

IndexBuffer* GraphicsDeviceGLES2::GetBoundIndexBuffer()
{
    IndexReverseMap::iterator it = _IndexReverseBuffers.find(_State->boundIndexBuffer);
    
    if (it != _IndexReverseBuffers.end())
        return it->second;
    
    return 0;
}

IndexBuffer* GraphicsDeviceGLES2::BindIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint indexBufferId = indexBuffer ? _IndexBuffers[indexBuffer] : 0;
    
    IndexBuffer* previousBuffer = GetBoundIndexBuffer();
    
    if (indexBufferId != _State->boundIndexBuffer)
    {
        _State->boundIndexBuffer = indexBufferId;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    }
    
    return previousBuffer;
}

void GraphicsDeviceGLES2::LockIndexBuffer(IndexBuffer* indexBuffer)
{
    GraphicsDevice::LockIndexBuffer(indexBuffer);
}

void GraphicsDeviceGLES2::UnlockIndexBuffer(IndexBuffer* indexBuffer, int numElements)
{
    if (numElements == 0)
        return;
        
    if (numElements == -1)
        numElements = indexBuffer->GetLength();
    
    IndexBuffer* previousBuffer = BindIndexBuffer(indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numElements, indexBuffer->GetData(), indexBuffer->GetBufferFormat() == kBufferFormatStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    BindIndexBuffer(previousBuffer);
}
    
Texture* GraphicsDeviceGLES2::CreateTexture()
{
    TextureGLES2* texture = new TextureGLES2(this);
    _Textures.push_back(texture);
    return texture;
}

void GraphicsDeviceGLES2::DestroyTexture(Texture* texture)
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

Texture* GraphicsDeviceGLES2::GetBoundTexture()
{
    for (TextureList::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        if ((*it)->_Texture == _State->boundTexture)
            return *it;
    }
    
    return 0;
}
    
Texture* GraphicsDeviceGLES2::BindTexture(Texture* texture)
{
    GLuint textureId = texture ? static_cast<TextureGLES2*>(texture)->_Texture : 0;
    
    Texture* previousTexture = GetBoundTexture();
    
    if (textureId != _State->boundTexture)
    {
        _State->boundTexture = textureId;
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
    
    return previousTexture;
}

Material* GraphicsDeviceGLES2::CreateMaterial()
{
    MaterialGLES2* material = new MaterialGLES2(this);
    _Materials.push_back(material);
    return material;
}

void GraphicsDeviceGLES2::DestroyMaterial(Material* material)
{
    for (MaterialList::iterator it = _Materials.begin(); it != _Materials.end(); ++it)
    {
        if (*it == material)
        {
            _Materials.erase(it);
            GraphicsDevice::DestroyMaterial(material);
            return;
        }
    }
}

Material* GraphicsDeviceGLES2::GetBoundMaterial()
{
    return _State->boundMaterial;
}

Material* GraphicsDeviceGLES2::BindMaterial(Material* material)
{
    Material* previousMaterial = GetBoundMaterial();
    
    if (material != _State->boundMaterial)
    {
        static_cast<MaterialGLES2*>(material)->Bind();
        _State->boundMaterial = material;
    }
    
    return previousMaterial;
}

void GraphicsDeviceGLES2::SetState(State state, bool enable)
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
    }
    
    if (enable)
    {
        glEnable(glState);
    } else {
        glDisable(glState);
    }
}

void GraphicsDeviceGLES2::SetBlendMode(Blend source, Blend destination)
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
            glSource = GL_BLEND_SRC_ALPHA;
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
            glDest = GL_BLEND_SRC_ALPHA;
            break;
    }
    
    glBlendFunc(glSource, glDest);
}

void GraphicsDeviceGLES2::SetViewport(glm::vec4 viewport)
{
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}

void GraphicsDeviceGLES2::DrawElements(ElementType elementType, int num)
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
