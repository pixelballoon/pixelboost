#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/vertexBuffer.h"

using namespace libpixel;

VertexBuffer::VertexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
    : _Device(device)
    , _BufferFormat(bufferFormat)
    , _VertexFormat(vertexFormat)
    , _Length(length)
{
    _Data = 0;
    _Locked = 0;
    
    switch (_VertexFormat)
    {
        case kVertexFormat_P_XY_RGBA:
            _Data = new Vertex_PXY_RGBA[length];
            break;
        case kVertexFormat_P_XYZ_UV:
            _Data = new Vertex_PXYZ_UV[length];
            break;
        case kVertexFormat_P_XYZ_RGBA:
            _Data = new Vertex_PXYZ_RGBA[length];
            break;
        case kVertexFormat_P_XYZ_RGBA_UV:
            _Data = new Vertex_PXYZ_RGBA_UV[length];
            break;
        case kVertexFormat_NP_XYZ_UV:
            _Data = new Vertex_NPXYZ_UV[length];
            break;
    }
}
    
VertexBuffer::~VertexBuffer()
{
    switch (_VertexFormat)
    {
        case kVertexFormat_P_XY_RGBA:
            delete[] static_cast<Vertex_PXY_RGBA*>(_Data);
            break;
        case kVertexFormat_P_XYZ_UV:
            delete[] static_cast<Vertex_PXYZ_UV*>(_Data);
            break;
        case kVertexFormat_P_XYZ_RGBA:
            delete[] static_cast<Vertex_PXYZ_RGBA*>(_Data);
            break;
        case kVertexFormat_P_XYZ_RGBA_UV:
            delete[] static_cast<Vertex_PXYZ_RGBA_UV*>(_Data);
            break;
        case kVertexFormat_NP_XYZ_UV:
            delete[] static_cast<Vertex_NPXYZ_UV*>(_Data);
            break;
    }
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
    return _Data;
}

void VertexBuffer::Unlock(int numElements)
{
    _Locked--;
    
    if (_Locked == 0)
    {
        _Device->UnlockVertexBuffer(this, numElements);
    }
}

void VertexBuffer::Bind()
{
    _Device->BindVertexBuffer(this);
}
