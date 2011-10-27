#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/vertexBuffer.h"

using namespace libpixel;

VertexBuffer::VertexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
    : _Device(device)
    , _BufferFormat(bufferFormat)
    , _VertexFormat(vertexFormat)
    , _Length(length)
{
    _Locked = 0;
    
    switch (vertexFormat)
    {
        case kVertexFormat_P_XYZ_UV:
            _Data = new Vertex_PXYZ_UV[length];
            break;
        case kVertexFormat_NP_XYZ_UV:
            _Data = new Vertex_NPXYZ_UV[length];
            break;
    }
}
    
VertexBuffer::~VertexBuffer()
{
    
}

BufferFormat VertexBuffer::GetBufferFormat()
{
    return _BufferFormat;
}

int VertexBuffer::GetLength()
{
    return _Length;
}

VertexFormat VertexBuffer::GetVertexFormat()
{
    return _VertexFormat;
}

void VertexBuffer::Lock()
{
    if (_Locked == 0)
    {
        _Device->LockVertexBuffer(this);
    }
    
    _Locked++;
}
    
void* VertexBuffer::GetData()
{
    if (_Locked)
    {
        return _Data;
    }
    
    return 0;
}

void VertexBuffer::Unlock()
{
    _Locked--;
    
    if (_Locked == 0)
    {
        _Device->UnlockVertexBuffer(this);
    }
}

void VertexBuffer::Bind()
{
    _Device->BindVertexBuffer(this);
}
