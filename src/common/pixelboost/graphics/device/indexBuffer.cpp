#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"

using namespace pb;

IndexBuffer::IndexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, int maxSize)
    : _Device(device)
    , _BufferFormat(bufferFormat)
    , _MaxSize(maxSize)
    , _CurrentSize(0)
{
    _Data = new unsigned short[maxSize];
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

int IndexBuffer::GetMaxSize()
{
    return _MaxSize;
}

int IndexBuffer::GetCurrentSize()
{
    return _CurrentSize;
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
        _CurrentSize = (numElements == -1) ? _MaxSize : numElements;
        _Device->UnlockIndexBuffer(this, numElements);
    }
}

#endif