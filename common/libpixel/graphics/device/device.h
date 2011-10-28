#pragma once

#include "libpixel/graphics/device/bufferFormats.h"

namespace libpixel
{

class Texture;
class IndexBuffer;
class VertexBuffer;

class GraphicsDevice
{
protected:
    GraphicsDevice();
    static GraphicsDevice* Create();
    
public:
    static GraphicsDevice* Instance();
    virtual ~GraphicsDevice();
    
    virtual VertexBuffer* CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length);
    virtual void DestroyVertexBuffer(VertexBuffer* buffer);
    virtual void BindVertexBuffer(VertexBuffer* buffer);
    
    virtual void LockVertexBuffer(VertexBuffer* buffer);
    virtual void UnlockVertexBuffer(VertexBuffer* buffer);
    
    virtual IndexBuffer* CreateIndexBuffer(BufferFormat bufferFormat, int length);
    virtual void DestroyIndexBuffer(IndexBuffer* buffer);
    virtual void BindIndexBuffer(IndexBuffer* buffer);
    
    virtual void LockIndexBuffer(IndexBuffer* vertexBuffer);
    virtual void UnlockIndexBuffer(IndexBuffer* vertexBuffer);
    
    virtual Texture* CreateTexture() = 0;
    virtual void RemoveTexture(Texture* texture) = 0;
    virtual void BindTexture(Texture* texture) = 0;
};
    
}

#ifdef LIBPIXEL_PLATFORM_IOS
    #define LIBPIXEL_GRAPHICS_OPENGLES1
    #define LIBPIXEL_GRAPHICS_PREMULTIPLIED_ALPHA
    #include "libpixel/graphics/device/gles1/device.h"
#endif
