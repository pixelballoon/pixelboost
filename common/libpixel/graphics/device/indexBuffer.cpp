#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/indexBuffer.h"

using namespace libpixel;

IndexBuffer::IndexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, int length)
    : _Device(device)
    , _BufferFormat(bufferFormat)
    , _Length(length)
{
    _Locked = 0;
}
    
IndexBuffer::~IndexBuffer()
{
    
}

BufferFormat IndexBuffer::GetBufferFormat()
{
    return _BufferFormat;
}

int IndexBuffer::GetLength()
{
    return _Length;
}

void IndexBuffer::Lock()
{
    if (_Locked == 0)
    {
        _Device->LockIndexBuffer(this);
    }
    
    _Locked++;
}
    
void* IndexBuffer::GetData()
{
    if (_Locked)
    {
        return _Data;
    }
    
    return 0;
}

void IndexBuffer::Unlock()
{
    _Locked--;
    
    if (_Locked == 0)
    {
        _Device->UnlockIndexBuffer(this);
    }
}

void IndexBuffer::Bind()
{
    _Device->BindIndexBuffer(this);
}
