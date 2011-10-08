#ifndef LIBPIXEL__GRAPHICS__DEVICE__GLES1__VERTEXBUFFERGLES1__H
#define LIBPIXEL__GRAPHICS__DEVICE__GLES1__VERTEXBUFFERGLES1__H

namespace libpixel
{

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
public:
    VertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int numElements);
    
    void* Lock();
    void Unlock();
    
public:
    virtual void RefillVertexBuffer();
    
private:
    BufferFormat _BufferFormat;
    VertexFormat _VertexFormat;
    int _NumElements;    
    
    friend class Device;
};

}

#endif