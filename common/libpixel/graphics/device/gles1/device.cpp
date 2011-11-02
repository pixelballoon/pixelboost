#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/indexBuffer.h"
#include "libpixel/graphics/device/texture.h"
#include "libpixel/graphics/device/vertexBuffer.h"

using namespace libpixel;

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1

#include <OpenGLES/ES1/gl.h>

struct libpixel::DeviceState
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
    GLuint vertexBufferId = _VertexBuffers[vertexBuffer];
    
//    if (vertexBufferId != _State->boundVertexBuffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        
        _State->boundVertexBuffer = vertexBufferId;
        
        switch (vertexBuffer->GetVertexFormat())
        {
            case kVertexFormat_P_XYZ_UV:
            {
                GLsizei stride = sizeof(Vertex_PXYZ_UV);
                glTexCoordPointer(2, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_UV, uv));
                glVertexPointer(3, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_UV, position));
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

void GraphicsDeviceGLES1::LockVertexBuffer(VertexBuffer* vertexBuffer)
{
    GraphicsDevice::LockVertexBuffer(vertexBuffer);
}

void GraphicsDeviceGLES1::UnlockVertexBuffer(VertexBuffer* vertexBuffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffers[vertexBuffer]);
    switch (vertexBuffer->GetVertexFormat())
    {
        case kVertexFormat_P_XYZ_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_PXYZ_UV) * vertexBuffer->GetLength(), vertexBuffer->GetData(), GL_STATIC_DRAW);
            break;
        }
        case kVertexFormat_P_XYZ_RGBA_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_PXYZ_RGBA_UV) * vertexBuffer->GetLength(), vertexBuffer->GetData(), GL_STATIC_DRAW);
            break;
        }
        case kVertexFormat_NP_XYZ_UV:
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_NPXYZ_UV) * vertexBuffer->GetLength(), vertexBuffer->GetData(), GL_STATIC_DRAW);
            break;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    GLuint indexBufferId = _IndexBuffers[indexBuffer];
    
    //if (indexBufferId != _State->boundIndexBuffer)
    {
        _State->boundIndexBuffer = indexBufferId;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    }
}

void GraphicsDeviceGLES1::LockIndexBuffer(IndexBuffer* indexBuffer)
{
    GraphicsDevice::LockIndexBuffer(indexBuffer);
}

void GraphicsDeviceGLES1::UnlockIndexBuffer(IndexBuffer* indexBuffer)
{
    GLuint indexBufferId = _IndexBuffers[indexBuffer];
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indexBuffer->GetLength(), indexBuffer->GetData(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    GLuint textureId = static_cast<TextureGLES1*>(texture)->_Texture;
    
    //if (textureId != _State->boundTexture)
    {
        _State->boundTexture = textureId;
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
}

#endif
