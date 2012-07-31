#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/debug/assert.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"

using namespace pb;

GraphicsDevice::GraphicsDevice()
    : _DisplayResolution(0,0)
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


void GraphicsDevice::SetDisplayResolution(glm::vec2 resolution)
{
    _DisplayResolution = resolution;
}

glm::vec2 GraphicsDevice::GetDisplayResolution()
{
    return _DisplayResolution;
}

void GraphicsDevice::SetDisplayDensity(float density)
{
    _DisplayDensity = density;
}

float GraphicsDevice::GetDisplayDensity()
{
    return _DisplayDensity;
}

unsigned char* GraphicsDevice::CaptureRenderBuffer()
{
    return 0;
}

void GraphicsDevice::ClearBuffers()
{
    
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

void GraphicsDevice::DestroyProgram(ShaderProgram* program)
{
    delete program;
}

void GraphicsDevice::OnContextLost()
{
    PbAssert(!"This device does not support the loss of the graphics context");
}

#endif
