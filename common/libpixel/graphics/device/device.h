#pragma once

namespace libpixel
{

class VertexBuffer;

class GraphicsDevice
{
protected:
    GraphicsDevice();
    
public:
    static GraphicsDevice* Create();
    virtual ~GraphicsDevice();
    
    // This does not take ownership of the VerteBuffer
    virtual void AddVertexBuffer(VertexBuffer* buffer);
    virtual void RemoveVertexBuffer(VertexBuffer* buffer);
    virtual void BindVertexBuffer(VertexBuffer* buffer);
    
    // This will return 0 if the device has been lost
    // The vertex buffer will receive a RefillVertexBuffer callback once the context is regained
    // Always check for null before using
    virtual void* LockVertexBuffer(VertexBuffer* buffer);
    virtual void UnlockVertexBuffer(VertexBuffer* buffer);
};
    
}

#ifdef LIBPIXEL_PLATFORM_IOS
    #define LIBPIXEL_GRAPHICS_OPENGLES1
    #include "libpixel/graphics/device/gles1/device.h"
#endif
