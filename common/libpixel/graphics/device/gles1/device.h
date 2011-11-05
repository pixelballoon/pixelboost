#pragma once

#include "libpixel/graphics/device/device.h"

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <map>

namespace libpixel
{

struct DeviceState;
class IndexBuffer;
class Texture;
class VertexBuffer;
    
class GraphicsDeviceGLES1 : public GraphicsDevice
{
public:
    GraphicsDeviceGLES1();
    virtual ~GraphicsDeviceGLES1();
    
public:
    virtual VertexBuffer* CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length);
    virtual void DestroyVertexBuffer(VertexBuffer* buffer);
    virtual void BindVertexBuffer(VertexBuffer* buffer);
    
    virtual void LockVertexBuffer(VertexBuffer* vertexBuffer);
    virtual void UnlockVertexBuffer(VertexBuffer* vertexBuffer);
    
    virtual IndexBuffer* CreateIndexBuffer(BufferFormat bufferFormat, int length);
    virtual void DestroyIndexBuffer(IndexBuffer* buffer);
    virtual void BindIndexBuffer(IndexBuffer* buffer);
    
    virtual void LockIndexBuffer(IndexBuffer* vertexBuffer);
    virtual void UnlockIndexBuffer(IndexBuffer* vertexBuffer);
    
    virtual Texture* CreateTexture();
    virtual void DestroyTexture(Texture* texture);
    virtual void BindTexture(Texture* texture);
    
public:
    virtual void DrawElements(ElementType elementType, int num);
    
private:
    DeviceState* _State;
    
    std::map<IndexBuffer*, GLuint> _IndexBuffers;
    std::map<VertexBuffer*, GLuint> _VertexBuffers;
};

}

#endif
