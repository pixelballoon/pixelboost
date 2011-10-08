#include "libpixel/graphics/device/gles1/deviceGLES1.h"

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1

namespace libpixel
{

GraphicsDevice* GraphicsDevice::Create()
{
    return new GLES1Device();
}

GLES1Device::GLES1Device()
{
    _BoundVertexBuffer = 0;
}

GLES1Device::~GLES1Device()
{
    
}

void GLES1Device::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
    
}

void GLES1Device::RemoveVertexBuffer(VertexBuffer* vertexBuffer)
{
    
}

void GLES1Device::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    
}

void* GLES1Device::LockVertexBuffer(VertexBuffer* vertexBuffer)
{
    return 0;
}

void GLES1Device::UnlockVertexBuffer(VertexBuffer* vertexBuffer)
{
    
}
    
}

#endif