#pragma once

#include "pixelboost/graphics/device/bufferFormats.h"

namespace pb
{
    
class GraphicsDevice;

struct Vertex_P3
{
    float position[3];
};
    
struct Vertex_P3_UV
{
    float position[3];
    float uv[2];
};

struct Vertex_P3_C4
{
    float position[3];
    float color[4];
};
    
struct Vertex_P3_C4_UV
{
    float position[3];
    float uv[2];
    float color[4];
};

struct Vertex_P3_N3_UV
{
    float position[3];
    float normal[3];
    float uv[2];
};
    
struct Vertex_P3_N3_UV_BW
{
    float position[3];
    float normal[3];
    float uv[2];
    char bones[4];
    float boneWeights[4];
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
