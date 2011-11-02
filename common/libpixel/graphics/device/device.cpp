#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/indexBuffer.h"
#include "libpixel/graphics/device/texture.h"
#include "libpixel/graphics/device/vertexBuffer.h"

using namespace libpixel;

GraphicsDevice::GraphicsDevice()
{
    
}

GraphicsDevice::~GraphicsDevice()
{
    
}

GraphicsDevice* GraphicsDevice::Instance()
{
    static GraphicsDevice* instance = GraphicsDevice::Create();
    return instance;
}
    
VertexBuffer* GraphicsDevice::CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
{
    return new VertexBuffer(this, bufferFormat, vertexFormat, length);
}

void GraphicsDevice::DestroyVertexBuffer(VertexBuffer* buffer)
{
    delete buffer;
}

void GraphicsDevice::BindVertexBuffer(VertexBuffer* buffer)
{
    
}
    
void GraphicsDevice::LockVertexBuffer(VertexBuffer* buffer)
{
    
}
    
void GraphicsDevice::UnlockVertexBuffer(VertexBuffer* buffer)
{
    
}
    
IndexBuffer* GraphicsDevice::CreateIndexBuffer(BufferFormat bufferFormat, int length)
{
    return new IndexBuffer(this, bufferFormat, length);
}

void GraphicsDevice::DestroyIndexBuffer(IndexBuffer* buffer)
{
    delete buffer;
}

void GraphicsDevice::BindIndexBuffer(IndexBuffer* buffer)
{
    
}

void GraphicsDevice::LockIndexBuffer(IndexBuffer* vertexBuffer)
{
    
}

void GraphicsDevice::UnlockIndexBuffer(IndexBuffer* vertexBuffer)
{
    
}

void GraphicsDevice::DestroyTexture(Texture* texture)
{
    delete texture;
}
