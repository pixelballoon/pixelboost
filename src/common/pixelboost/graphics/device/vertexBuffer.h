#pragma once

#include "pixelboost/graphics/device/bufferFormats.h"

namespace pb
{
    
class GraphicsDevice;

struct Vertex_PXY_RGBA
{
    float position[2];
    float color[4];
};

struct Vertex_PXYZ
{
    float position[3];
    float __padding;
};
    
struct Vertex_PXYZ_UV
{
    float position[3];
    float uv[2];
    float __padding[3]; // for 32-byte alignment
};

struct Vertex_PXYZ_RGBA
{
    float position[3];
    float color[4];
};
    
struct Vertex_PXYZ_RGBA_UV
{
    float position[3];
    float uv[2];
    float color[4];
    float __padding[2]; // for 48-byte alignment
};

struct Vertex_NPXYZ_UV
{
    float position[3];
    float normal[3];
    float uv[2];
};

class VertexBuffer
{
protected:
    VertexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, VertexFormat vertexFormat, int maxSize);
    ~VertexBuffer();
    
public:    
    BufferFormat GetBufferFormat();
    VertexFormat GetVertexFormat();
    
    int GetMaxSize();
    int GetCurrentSize();
    
    void* GetData();
    
    void Lock();
    void Unlock(int numElements=-1);
    
private:
    GraphicsDevice* _Device;
    
    BufferFormat _BufferFormat;
    VertexFormat _VertexFormat;
    int _MaxSize;
    int _CurrentSize;
    
    void* _Data;
    
    int _Locked;
    
    friend class GraphicsDevice;
};

}
