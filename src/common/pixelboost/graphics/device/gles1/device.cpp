#include "glm/gtc/type_ptr.hpp"

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"

using namespace pb;

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1

#include <OpenGLES/ES1/gl.h>

struct pb::DeviceState
{
    DeviceState()
    {
        boundIndexBuffer = 0;
        boundTexture = 0;
        boundVertexBuffer = 0;
    }
    
    GLuint boundIndexBuffer;
    GLuint boundTexture;
    GLuint boundVertexBuffer;
};

GraphicsDevice* GraphicsDevice::Create()
{
    return new GraphicsDeviceGLES1();
}

GraphicsDeviceGLES1::GraphicsDeviceGLES1()
{
    _State = new DeviceState();
}

GraphicsDeviceGLES1::~GraphicsDeviceGLES1()
{
    delete _State;
}

unsigned char* GraphicsDeviceGLES1::CaptureRenderBuffer()
{    
    int width, height;
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &width);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &height);

    unsigned char* buffer = new unsigned char[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    
    return buffer;
}

VertexBuffer* GraphicsDeviceGLES1::CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
{
    VertexBuffer* vertexBuffer = GraphicsDevice::CreateVertexBuffer(bufferFormat, vertexFormat, length);
    
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    
    _VertexBuffers[vertexBuffer] = buffer;
    _VertexReverseBuffers[buffer] = vertexBuffer;
    
    return vertexBuffer;
}

void GraphicsDeviceGLES1::DestroyVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint buffer = _VertexBuffers[vertexBuffer];
    glDeleteBuffers(1, &buffer);
    
    _VertexBuffers.erase(vertexBuffer);
    _VertexReverseBuffers.erase(buffer);
    
    GraphicsDevice::DestroyVertexBuffer(vertexBuffer);
}

VertexBuffer* GraphicsDeviceGLES1::GetBoundVertexBuffer()
{
    VertexReverseMap::iterator it = _VertexReverseBuffers.find(_State->boundVertexBuffer);
    
    if (it != _VertexReverseBuffers.end())
        return it->second;
    
    return 0;
}

VertexBuffer* GraphicsDeviceGLES1::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint vertexBufferId = vertexBuffer ? _VertexBuffers[vertexBuffer] : 0;
    
    int previousBinding = _State->boundVertexBuffer;
    
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
                    glColorPointer(4, GL_FLOAT, stride, (void*)offsetof(Vertex_PXY_RGBA, color));
                    glVertexPointer(2, GL_FLOAT, stride, (void*)offsetof(Vertex_PXY_RGBA, position));
                    break;
                }
                case kVertexFormat_P_XYZ_UV:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_UV);
                    glTexCoordPointer(2, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_UV, uv));
                    glVertexPointer(3, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_UV, position));
                    break;
                }
                case kVertexFormat_P_XYZ_RGBA:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_RGBA);
                    glColorPointer(4, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_RGBA, color));
                    glVertexPointer(3, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_RGBA, position));
                    break;
                }
                case kVertexFormat_P_XYZ_RGBA_UV:
                {
                    GLsizei stride = sizeof(Vertex_PXYZ_RGBA_UV);
                    glColorPointer(4, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, color));
                    glTexCoordPointer(2, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, uv));
                    glVertexPointer(3, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_RGBA_UV, position));
                    break;
                }
                case kVertexFormat_NP_XYZ_UV:
                {
                    GLsizei stride = sizeof(Vertex_NPXYZ_UV);
                    glNormalPointer(GL_FLOAT, stride, (void*)offsetof(Vertex_NPXYZ_UV, normal));
                    glTexCoordPointer(2, GL_FLOAT, stride, (void*)offsetof(Vertex_NPXYZ_UV, uv));
                    glVertexPointer(3, GL_FLOAT, stride, (void*)offsetof(Vertex_NPXYZ_UV, position));
                    break;
                }
            }
        }
    }
    
    VertexReverseMap::iterator it = _VertexReverseBuffers.find(previousBinding);
    
    if (it != _VertexReverseBuffers.end())
        return it->second;
    
    return 0;
}

void GraphicsDeviceGLES1::LockVertexBuffer(VertexBuffer* vertexBuffer)
{
    GraphicsDevice::LockVertexBuffer(vertexBuffer);
}

void GraphicsDeviceGLES1::UnlockVertexBuffer(VertexBuffer* vertexBuffer, int numElements)
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
    
IndexBuffer* GraphicsDeviceGLES1::CreateIndexBuffer(BufferFormat bufferFormat, int length)
{
    IndexBuffer* indexBuffer = GraphicsDevice::CreateIndexBuffer(bufferFormat, length);
    
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    
    _IndexBuffers[indexBuffer] = buffer;
    _IndexReverseBuffers[buffer] = indexBuffer;
    
    return indexBuffer;
}

void GraphicsDeviceGLES1::DestroyIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint buffer = _IndexBuffers[indexBuffer];
    glDeleteBuffers(1, &buffer);
    
    _IndexBuffers.erase(indexBuffer);
    _IndexReverseBuffers.erase(buffer);
    
    GraphicsDevice::DestroyIndexBuffer(indexBuffer);
}

IndexBuffer* GraphicsDeviceGLES1::GetBoundIndexBuffer()
{
    IndexReverseMap::iterator it = _IndexReverseBuffers.find(_State->boundIndexBuffer);
    
    if (it != _IndexReverseBuffers.end())
        return it->second;
    
    return 0;
}

IndexBuffer* GraphicsDeviceGLES1::BindIndexBuffer(IndexBuffer* indexBuffer)
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

void GraphicsDeviceGLES1::LockIndexBuffer(IndexBuffer* indexBuffer)
{
    GraphicsDevice::LockIndexBuffer(indexBuffer);
}

void GraphicsDeviceGLES1::UnlockIndexBuffer(IndexBuffer* indexBuffer, int numElements)
{
    if (numElements == 0)
        return;
        
    if (numElements == -1)
        numElements = indexBuffer->GetLength();
    
    IndexBuffer* previousBuffer = BindIndexBuffer(indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numElements, indexBuffer->GetData(), indexBuffer->GetBufferFormat() == kBufferFormatStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    BindIndexBuffer(previousBuffer);
}
    
Texture* GraphicsDeviceGLES1::CreateTexture()
{
    TextureGLES1* texture = new TextureGLES1(this);
    _Textures.push_back(texture);
    return texture;
}

void GraphicsDeviceGLES1::DestroyTexture(Texture* texture)
{
    GraphicsDevice::DestroyTexture(texture);
}

Texture* GraphicsDeviceGLES1::GetBoundTexture()
{
    for (TextureList::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        if (static_cast<TextureGLES1*>(*it)->_Texture == _State->boundTexture)
            return *it;
    }
    
    return 0;
}
    
Texture* GraphicsDeviceGLES1::BindTexture(Texture* texture)
{
    GLuint textureId = texture ? static_cast<TextureGLES1*>(texture)->_Texture : 0;
    
    Texture* previousTexture = GetBoundTexture();
    
    if (textureId != _State->boundTexture)
    {
        _State->boundTexture = textureId;
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
    
    return previousTexture;
}

void GraphicsDeviceGLES1::SetViewport(glm::vec4 viewport)
{
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}

void GraphicsDeviceGLES1::SetMatrix(MatrixType matrixType, glm::mat4x4 matrix)
{
    switch (matrixType)
    {
        case kMatrixTypeModelView:
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(glm::value_ptr(matrix));
            break;
        case kMatrixTypeProjection:
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(glm::value_ptr(matrix));
            break;
    }
}

void GraphicsDeviceGLES1::DrawElements(ElementType elementType, int num)
{
    GLuint type;
    
    switch (elementType)
    {
        case kElementLines:
        {
            type = GL_LINES;
            break;
        }
        case kElementTriangles:
        {
            type = GL_TRIANGLES;
            break;
        }
    }
    
    glDrawElements(type, num, GL_UNSIGNED_SHORT, (void*)0);
}

#endif
