#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/bufferFormats.h"

namespace pb
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
    
    void SetDisplayResolution(glm::vec2 resolution);
    glm::vec2 GetDisplayResolution();
    
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
    
    enum MatrixType
    {
        kMatrixTypeProjection,
        kMatrixTypeModelView,
    };
    
    virtual void SetMatrix(MatrixType matrixType, glm::mat4x4 matrix) = 0;
    
public:
    enum ElementType
    {
        kElementLines,
        kElementTriangles,
    };
    
    virtual void DrawElements(ElementType elementType, int num) = 0;
    
private:
    glm::vec2 _DisplayResolution;
};
    
}

#ifdef PIXELBOOST_PLATFORM_IOS
    #define PIXELBOOST_GRAPHICS_OPENGLES1
    #include "pixelboost/graphics/device/gles1/device.h"
#endif

#ifdef PIXELBOOST_PLATFORM_OSX
    #define PIXELBOOST_GRAPHICS_OPENGL2
    #include "pixelboost/graphics/device/gl2/device.h"
#endif

#endif
