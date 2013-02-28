#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/debug/assert.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"

using namespace pb;

RenderBufferCapture::RenderBufferCapture(unsigned char* data, int width, int height)
{
    Data = data;
    Width = width;
    Height = height;
}

RenderBufferCapture::~RenderBufferCapture()
{
    delete[] Data;
}

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
    onDisplayResolutionChanged(resolution);
}

glm::vec2 GraphicsDevice::GetDisplayResolution()
{
    return _DisplayResolution;
}

void GraphicsDevice::SetDisplayDensity(float density)
{
    _DisplayDensity = density;
    onDisplayDensityChanged(density);
}

float GraphicsDevice::GetDisplayDensity()
{
    return _DisplayDensity;
}

std::shared_ptr<RenderBufferCapture> GraphicsDevice::CaptureRenderBuffer()
{
    return std::shared_ptr<RenderBufferCapture>(new RenderBufferCapture(0,0,0));
}

void GraphicsDevice::SetClearColor(glm::vec4 color)
{
    
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
