#pragma once

#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2
    #ifdef PIXELBOOST_PLATFORM_ANDROID
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>
    #else
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #endif
#endif

#ifdef PIXELBOOST_GRAPHICS_OPENGL2
    #include <OpenGL/gl.h>
#endif

#include <map>
#include <vector>

namespace pb
{

struct DeviceState;
class IndexBuffer;
class Texture;
class TextureGLES2;
class VertexBuffer;
    
struct DeviceState
{
    DeviceState();
    
    void Reset();
    
    GLuint boundIndexBuffer;
    GLuint boundTexture;
    GLuint boundVertexBuffer;
    GLuint boundProgram;
};
    
class GraphicsDeviceGL : public GraphicsDevice
{
public:
    GraphicsDeviceGL();
    virtual ~GraphicsDeviceGL();
    
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
    
    virtual ShaderProgram* CreateProgram();
    virtual void DestroyProgram(ShaderProgram* program);
    virtual ShaderProgram* GetBoundProgram();
    virtual ShaderProgram* BindProgram(ShaderProgram* program);
    
public:
    virtual void OnContextLost();
    
    virtual void SetState(State state, bool enable);
    virtual void SetBlendMode(Blend source, Blend destination);
    
    virtual void SetViewport(glm::vec4 viewport);
    virtual void SetScissor(glm::vec4 scissor);
    
public:
    virtual void DrawElements(ElementType elementType, int num);
    
private:
    DeviceState _CurrentState;
    
    typedef std::vector<ShaderProgram*> ProgramList;
    typedef std::vector<Texture*> TextureList;
    
    ProgramList _Programs;
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
