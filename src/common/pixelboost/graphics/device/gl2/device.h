#pragma once

#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL2

#include <OpenGL/gl.h>

#include <map>
#include <vector>

namespace pb
{

struct DeviceState;
class IndexBuffer;
class Texture;
class VertexBuffer;
    
class GraphicsDeviceGL2 : public GraphicsDevice
{
public:
    GraphicsDeviceGL2();
    virtual ~GraphicsDeviceGL2();
    
public:
    virtual unsigned char* CaptureRenderBuffer();
    
    virtual VertexBuffer* CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length);
    virtual void DestroyVertexBuffer(VertexBuffer* buffer);
    virtual VertexBuffer* GetBoundVertexBuffer();
    virtual VertexBuffer* BindVertexBuffer(VertexBuffer* buffer);
    
    virtual void LockVertexBuffer(VertexBuffer* vertexBuffer);
    virtual void UnlockVertexBuffer(VertexBuffer* vertexBuffer, int numElements=-1);
    
    virtual IndexBuffer* CreateIndexBuffer(BufferFormat bufferFormat, int length);
    virtual void DestroyIndexBuffer(IndexBuffer* buffer);
    virtual IndexBuffer* GetBoundIndexBuffer();
    virtual IndexBuffer* BindIndexBuffer(IndexBuffer* buffer);
    
    virtual void LockIndexBuffer(IndexBuffer* vertexBuffer);
    virtual void UnlockIndexBuffer(IndexBuffer* vertexBuffer, int numElements=-1);
    
    virtual Texture* CreateTexture();
    virtual void DestroyTexture(Texture* texture);
    virtual Texture* GetBoundTexture();
    virtual Texture* BindTexture(Texture* texture);
    
public:
    virtual void DrawElements(ElementType elementType, int num);
    
private:
    DeviceState* _State;
    
    typedef std::vector<Texture*> TextureList;
    
    TextureList _Textures;
    
    typedef std::map<IndexBuffer*, GLuint> IndexMap;
    typedef std::map<VertexBuffer*, GLuint> VertexMap;
    typedef std::map<GLuint, IndexBuffer*> IndexReverseMap;
    typedef std::map<GLuint, VertexBuffer*> VertexReverseMap;
    
    IndexMap _IndexBuffers;
    VertexMap _VertexBuffers;
    IndexReverseMap _IndexReverseBuffers;
    VertexReverseMap _VertexReverseBuffers;
};

}

#endif
