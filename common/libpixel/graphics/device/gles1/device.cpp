#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/texture.h"

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1

namespace libpixel
{

GraphicsDevice* GraphicsDevice::Create()
{
    return new GraphicsDeviceGLES1();
}

GraphicsDeviceGLES1::GraphicsDeviceGLES1()
{
    _BoundVertexBuffer = 0;
}

GraphicsDeviceGLES1::~GraphicsDeviceGLES1()
{
    
}

VertexBuffer* GraphicsDeviceGLES1::CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int numElements)
{
    VertexBuffer* vertexBuffer = new VertexBuffer(this, bufferFormat, vertexFormat, numElements);
    _VertexBuffers[vertexBuffer] = 0;
    return vertexBuffer;
}

void GraphicsDeviceGLES1::RemoveVertexBuffer(VertexBuffer* vertexBuffer)
{
    
}

void GraphicsDeviceGLES1::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    
}

void* GraphicsDeviceGLES1::LockVertexBuffer(VertexBuffer* vertexBuffer)
{
    return 0;
}

void GraphicsDeviceGLES1::UnlockVertexBuffer(VertexBuffer* vertexBuffer)
{
    
}
    
Texture* GraphicsDeviceGLES1::CreateTexture()
{
    return new TextureGLES1();
}

void GraphicsDeviceGLES1::RemoveTexture(Texture* texture)
{
    
}
    
void GraphicsDeviceGLES1::BindTexture(Texture* texture)
{
    
}
    
}

#endif
