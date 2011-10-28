#pragma once

#include "libpixel/graphics/device/bufferFormats.h"

namespace libpixel
{
    
class GraphicsDevice;

class IndexBuffer
{
protected:
    IndexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, int length);
    ~IndexBuffer();
    
public:    
    BufferFormat GetBufferFormat();
    int GetLength();
    
    unsigned short* GetData();
    
    void Lock();
    void Unlock();
    
    void Bind();
    
private:
    GraphicsDevice* _Device;
    
    BufferFormat _BufferFormat;
    int _Length;
    
    unsigned short* _Data;
    
    int _Locked;
    
    friend class GraphicsDevice;
};

}
