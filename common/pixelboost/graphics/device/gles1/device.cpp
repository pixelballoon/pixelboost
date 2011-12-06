#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"

using namespace pixelboost;

#ifdef PIXELBOOST_GRAPHICS_OPENGLES1

#include <OpenGLES/ES1/gl.h>

struct pixelboost::DeviceState
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

VertexBuffer* GraphicsDeviceGLES1::CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
{
    VertexBuffer* vertexBuffer = GraphicsDevice::CreateVertexBuffer(bufferFormat, vertexFormat, length);
    
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);

    
    _VertexBuffers[vertexBuffer] = buffer;
    
    return vertexBuffer;
}

void GraphicsDeviceGLES1::DestroyVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint buffer = _VertexBuffers[vertexBuffer];
    glDeleteBuffers(1, &buffer);
    
    GraphicsDevice::DestroyVertexBuffer(vertexBuffer);
}

void GraphicsDeviceGLES1::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    GLuint vertexBufferId = vertexBuffer ? _VertexBuffers[vertexBuffer] : 0;
    
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
    
    BindVertexBuffer(vertexBuffer);
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
}
    
IndexBuffer* GraphicsDeviceGLES1::CreateIndexBuffer(BufferFormat bufferFormat, int length)
{
    IndexBuffer* indexBuffer = GraphicsDevice::CreateIndexBuffer(bufferFormat, length);
    
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    
    _IndexBuffers[indexBuffer] = buffer;
    
    return indexBuffer;
}

void GraphicsDeviceGLES1::DestroyIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint buffer = _IndexBuffers[indexBuffer];
    glDeleteBuffers(1, &buffer);
    
    GraphicsDevice::DestroyIndexBuffer(indexBuffer);
}

void GraphicsDeviceGLES1::BindIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint indexBufferId = indexBuffer ? _IndexBuffers[indexBuffer] : 0;
    
    if (indexBufferId != _State->boundIndexBuffer)
    {
        _State->boundIndexBuffer = indexBufferId;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    }
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
    
    BindIndexBuffer(indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numElements, indexBuffer->GetData(), indexBuffer->GetBufferFormat() == kBufferFormatStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}
    
Texture* GraphicsDeviceGLES1::CreateTexture()
{
    return new TextureGLES1(this);
}

void GraphicsDeviceGLES1::DestroyTexture(Texture* texture)
{
    GraphicsDevice::DestroyTexture(texture);
}
    
void GraphicsDeviceGLES1::BindTexture(Texture* texture)
{
    GLuint textureId = texture ? static_cast<TextureGLES1*>(texture)->_Texture : 0;
    
    if (textureId != _State->boundTexture)
    {
        _State->boundTexture = textureId;
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
}
enum ElementType
{
    kElementLines,
    kElementTriangles,
};

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
