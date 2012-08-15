#pragma once

#include "pixelboost/graphics/device/bufferFormats.h"

namespace pb
{
    
class GraphicsDevice;

class IndexBuffer
{
protected:
    IndexBuffer(GraphicsDevice* device, BufferFormat bufferFormat, int length);
    ~IndexBuffer();
    
public:    
    BufferFormat GetBufferFormat();

    int GetMaxSize();
    int GetCurrentSize();
    
    unsigned short* GetData();
    
    void Lock();
    void Unlock(int numElements=-1);
    
private:
    GraphicsDevice* _Device;
    
    BufferFormat _BufferFormat;
    int _MaxSize;
    int _CurrentSize;
    
    unsigned short* _Data;
    
    int _Locked;
    
    friend class GraphicsDevice;
};

}
