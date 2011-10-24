#pragma once

#include "libpixel/graphics/device/device.h"

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1

#include "libpixel/graphics/device/vertexBuffer.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <map>

namespace libpixel
{

class Texture;
class VertexBuffer;
    
class GraphicsDeviceGLES1 : public GraphicsDevice
{
public:
    GraphicsDeviceGLES1();
    virtual ~GraphicsDeviceGLES1();
    
    virtual VertexBuffer* CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int numElements);
    virtual void RemoveVertexBuffer(VertexBuffer* buffer);
    virtual void BindVertexBuffer(VertexBuffer* buffer);
    
    virtual void* LockVertexBuffer(VertexBuffer* vertexBuffer);
    virtual void UnlockVertexBuffer(VertexBuffer* vertexBuffer);
    
    virtual Texture* CreateTexture();
    virtual void RemoveTexture(Texture* texture);
    virtual void BindTexture(Texture* texture);
    
private:
    GLuint _BoundVertexBuffer;
    
    std::map<VertexBuffer*, GLuint> _VertexBuffers;
};

}

#endif
