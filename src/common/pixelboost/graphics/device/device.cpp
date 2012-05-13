#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"

using namespace pb;

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

unsigned char* GraphicsDevice::CaptureRenderBuffer()
{
    return 0;
}
    
VertexBuffer* GraphicsDevice::CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
{
    return new VertexBuffer(this, bufferFormat, vertexFormat, length);
}

void GraphicsDevice::DestroyVertexBuffer(VertexBuffer* buffer)
{
    delete buffer;
}

void GraphicsDevice::LockVertexBuffer(VertexBuffer* buffer)
{
    
}
    
void GraphicsDevice::UnlockVertexBuffer(VertexBuffer* buffer, int numElements)
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

void GraphicsDevice::LockIndexBuffer(IndexBuffer* vertexBuffer)
{
    
}

void GraphicsDevice::UnlockIndexBuffer(IndexBuffer* vertexBuffer, int numElements)
{
    
}

void GraphicsDevice::DestroyTexture(Texture* texture)
{
    delete texture;
}

#endif
