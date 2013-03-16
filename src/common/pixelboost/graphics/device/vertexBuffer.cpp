#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/vertexBuffer.h"

using namespace pb;

VertexBuffer::VertexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, VertexFormat vertexFormat, int length)
    : _Device(device)
    , _BufferFormat(bufferFormat)
    , _VertexFormat(vertexFormat)
    , _CurrentSize(0)
    , _MaxSize(length)
{
    _Data = 0;
    _Locked = 0;
    
    switch (_VertexFormat)
    {
        case kVertexFormat_P3:
            _Data = new Vertex_P3[length];
            break;
        case kVertexFormat_P3_UV:
            _Data = new Vertex_P3_UV[length];
            break;
        case kVertexFormat_P3_C4:
            _Data = new Vertex_P3_C4[length];
            break;
        case kVertexFormat_P3_C4_UV:
            _Data = new Vertex_P3_C4_UV[length];
            break;
        case kVertexFormat_P3_N3_UV:
            _Data = new Vertex_P3_N3_UV[length];
            break;
        case kVertexFormat_P3_N3_UV_BW:
            _Data = new Vertex_P3_N3_UV_BW[length];
            break;
    }
}

VertexBuffer::~VertexBuffer()
{
    switch (_VertexFormat)
    {
        case kVertexFormat_P3:
            delete[] static_cast<Vertex_P3*>(_Data);
            break;
        case kVertexFormat_P3_UV:
            delete[] static_cast<Vertex_P3_UV*>(_Data);
            break;
        case kVertexFormat_P3_C4:
            delete[] static_cast<Vertex_P3_C4*>(_Data);
            break;
        case kVertexFormat_P3_C4_UV:
            delete[] static_cast<Vertex_P3_C4_UV*>(_Data);
            break;
        case kVertexFormat_P3_N3_UV:
            delete[] static_cast<Vertex_P3_N3_UV*>(_Data);
            break;
        case kVertexFormat_P3_N3_UV_BW:
            delete[] static_cast<Vertex_P3_N3_UV_BW*>(_Data);
            break;

    }
}

BufferFormat VertexBuffer::GetBufferFormat()
{
    return _BufferFormat;
}

VertexFormat VertexBuffer::GetVertexFormat()
{
    return _VertexFormat;
}

int VertexBuffer::GetMaxSize()
{
    return _MaxSize;
}

int VertexBuffer::GetCurrentSize()
{
    return _CurrentSize;
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
        _CurrentSize = (numElements == -1) ? _MaxSize : numElements;
        _Device->UnlockVertexBuffer(this, numElements);
    }
}
