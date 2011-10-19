#pragma once

namespace libpixel
{
    
class GraphicsDevice;

enum BufferFormat
{
    kBufferFormatStatic,
    kBufferFormatDynamic,
};

enum VertexFormat
{
    kVertexFormat_P_XYZ_UV,
    kVertexFormat_NP_XYZ_UV,
};

struct Vertex_PXYZ_UV
{
    float position[3];
    float uv[2];
    float __padding[3]; // for 32-byte alignment
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
    VertexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, VertexFormat vertexFormat, int numElements);
    
public:    
    void* Lock();
    void Unlock();
    
public:
    virtual void RefillVertexBuffer();
    
private:
    GraphicsDevice* _Device;
    
    BufferFormat _BufferFormat;
    VertexFormat _VertexFormat;
    int _NumElements;    
    
    friend class GraphicsDevice;
    friend class GraphicsDeviceGLES1;
};

}
