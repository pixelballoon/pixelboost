#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/device/bufferFormats.h"

namespace pixelboost
{

class Texture;
class IndexBuffer;
class VertexBuffer;

class GraphicsDevice
{
protected:
    GraphicsDevice();
    static GraphicsDevice* Create();
    
public:
    static GraphicsDevice* Instance();
    virtual ~GraphicsDevice();
    
    virtual unsigned char* CaptureRenderBuffer();
    
    virtual VertexBuffer* CreateVertexBuffer(BufferFormat bufferFormat, VertexFormat vertexFormat, int length);
    virtual void DestroyVertexBuffer(VertexBuffer* buffer);
    virtual VertexBuffer* GetBoundVertexBuffer() = 0;
    virtual VertexBuffer* BindVertexBuffer(VertexBuffer* buffer) = 0;
    
    virtual void LockVertexBuffer(VertexBuffer* buffer);
    virtual void UnlockVertexBuffer(VertexBuffer* buffer, int numElements=-1);
    
    virtual IndexBuffer* CreateIndexBuffer(BufferFormat bufferFormat, int length);
    virtual void DestroyIndexBuffer(IndexBuffer* buffer);
    virtual IndexBuffer* GetBoundIndexBuffer() = 0;
    virtual IndexBuffer* BindIndexBuffer(IndexBuffer* buffer) = 0;
    
    virtual void LockIndexBuffer(IndexBuffer* vertexBuffer);
    virtual void UnlockIndexBuffer(IndexBuffer* vertexBuffer, int numElements=-1);
    
    virtual Texture* CreateTexture() = 0;
    virtual void DestroyTexture(Texture* texture);
    virtual Texture* GetBoundTexture() = 0;
    virtual Texture* BindTexture(Texture* texture) = 0;
    
public:
    enum ElementType
    {
        kElementLines,
        kElementTriangles,
    };
    
    virtual void DrawElements(ElementType elementType, int num) = 0;
};
    
}

#ifdef PIXELBOOST_PLATFORM_IOS
    #define PIXELBOOST_GRAPHICS_OPENGLES1
    #define PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
#endif

#ifdef PIXELBOOST_PLATFORM_OSX
    #define PIXELBOOST_GRAPHICS_OPENGL
    #define PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
    #include <OpenGL/gl.h>
#endif

#endif
