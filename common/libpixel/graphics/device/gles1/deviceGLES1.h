#ifndef LIBPIXEL__GRAPHICS__DEVICE__GLES1__DEVICEGLES1__H
#define LIBPIXEL__GRAPHICS__DEVICE__GLES1__DEVICEGLES1__H

#include "libpixel/graphics/device/device.h"

#ifdef LIBPIXEL_GRAPHICS_OPENGLES1

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <map>

namespace libpixel
{

class VertexBuffer;
    
class GLES1Device : public GraphicsDevice
{
public:
    GLES1Device();
    virtual ~GLES1Device();
    
    virtual void AddVertexBuffer(VertexBuffer* buffer);
    virtual void RemoveVertexBuffer(VertexBuffer* buffer);
    virtual void BindVertexBuffer(VertexBuffer* buffer);
    
    virtual void* LockVertexBuffer(VertexBuffer* vertexBuffer);
    virtual void UnlockVertexBuffer(VertexBuffer* vertexBuffer);
    
private:
    GLuint _BoundVertexBuffer;
    
    std::map<VertexBuffer*, GLuint> _VertexBuffers;
};

}

#endif

#endif