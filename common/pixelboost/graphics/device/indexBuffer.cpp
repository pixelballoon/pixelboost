#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"

using namespace pixelboost;

IndexBuffer::IndexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, int length)
    : _Device(device)
    , _BufferFormat(bufferFormat)
    , _Length(length)
{
    _Data = new unsigned short[length];
    _Locked = 0;
}
    
IndexBuffer::~IndexBuffer()
{
    delete[] _Data;
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
    
unsigned short* IndexBuffer::GetData()
{
    return _Data;
}

void IndexBuffer::Unlock(int numElements)
{
    _Locked--;
    
    if (_Locked == 0)
    {
        _Device->UnlockIndexBuffer(this, numElements);
    }
}

void IndexBuffer::Bind()
{
    _Device->BindIndexBuffer(this);
}
