#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/bufferFormats.h"

namespace pb
{

class Texture;
class IndexBuffer;
class Material;
class ShaderProgram;
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
    
    void SetDisplayDensity(float density);
    float GetDisplayDensity();
    
    virtual unsigned char* CaptureRenderBuffer();

    virtual void ClearBuffers();
    
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
    virtual Texture* BindTexture(Texture* texture, bool force=false) = 0;
    
    virtual ShaderProgram* CreateProgram() = 0;
    virtual void DestroyProgram(ShaderProgram* program);
    virtual ShaderProgram* GetBoundProgram() = 0;
    virtual ShaderProgram* BindProgram(ShaderProgram* program) = 0;

public:
    enum Blend
    {
        kBlendOne,
        kBlendSourceAlpha,
        kBlendOneMinusSourceAlpha,
    };
    
    enum State
    {
        kStateBlend,
        kStateDepthTest,
        kStateTexture2D,
        kStateScissor,
    };
    
public:
    virtual void OnContextLost();
    
    virtual void SetState(State state, bool enable) = 0;
    virtual void SetBlendMode(Blend source, Blend destination) = 0;
    
    virtual void SetViewport(glm::vec4 viewport) = 0;
    virtual void SetScissor(glm::vec4 scissor) = 0;
    
public:
    enum ElementType
    {
        kElementLines,
        kElementLineLoop,
        kElementTriangles,
        kElementTriangleFan,
    };
    
    virtual void DrawElements(ElementType elementType, int num) = 0;
    
private:
    glm::vec2 _DisplayResolution;
    float _DisplayDensity;
};
    
}

#ifdef PIXELBOOST_PLATFORM_IOS
    #define PIXELBOOST_GRAPHICS_OPENGL
    #define PIXELBOOST_GRAPHICS_OPENGLES2
#endif

#ifdef PIXELBOOST_PLATFORM_NACL
    #define PIXELBOOST_GRAPHICS_OPENGL
    #define PIXELBOOST_GRAPHICS_OPENGLES2
#endif

#ifdef PIXELBOOST_PLATFORM_ANDROID
    #define PIXELBOOST_GRAPHICS_HANDLE_CONTEXT_LOST
    #define PIXELBOOST_GRAPHICS_OPENGL
    #define PIXELBOOST_GRAPHICS_OPENGLES2
#endif

#ifdef PIXELBOOST_PLATFORM_OSX
    #define PIXELBOOST_GRAPHICS_OPENGL
    #define PIXELBOOST_GRAPHICS_OPENGL2
#endif

#ifdef PIXELBOOST_PLATFORM_WINDOWS
	#define PIXELBOOST_GRAPHICS_OPENGL
    #define PIXELBOOST_GRAPHICS_OPENGL2
#endif

#endif
