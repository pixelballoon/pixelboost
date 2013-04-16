#pragma once

#include "pixelboost/graphics/device/device.h"

#ifdef PIXELBOOST_GRAPHICS_OPENGL

#ifdef PIXELBOOST_GRAPHICS_OPENGLES2
    #if defined(PIXELBOOST_PLATFORM_OSX) or defined(PIXELBOOST_PLATFORM_IOS)
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #else
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>
    #endif
#endif

#ifdef PIXELBOOST_GRAPHICS_OPENGL2
    #ifdef PIXELBOOST_PLATFORM_OSX
        #include <OpenGL/gl.h>
    #else
        #include <GL/gl.h>
        #include <GL/glew.h>
	#endif
#endif

#include <map>
#include <vector>

#include "pixelboost/graphics/device/program.h"

namespace pb
{
    
static const int kNumTextureUnits = 32;

struct DeviceState;
class IndexBuffer;
class Texture;
class TextureGLES2;
class VertexBuffer;
    
struct DeviceState
{
    DeviceState();
    
    void Reset();
    
    void UpdateAll(DeviceState& deviceState);
    void UpdateIndexBuffer(DeviceState& state);
    void UpdateVertexBuffer(DeviceState& state);
    void UpdateTexture(DeviceState& state);
    void UpdateStates(DeviceState& state);
    
    GLuint boundIndexBuffer;
    GLuint boundTexture[kNumTextureUnits];
    GLuint boundVertexBuffer;
    GLuint boundProgram;
    
    typedef std::map<GLuint, bool> StateMap;
    StateMap states;
    bool shaderAttribute[kShaderAttributeCount];
    
    friend class GraphicsDeviceGL;
};
    
class GraphicsDeviceGL : public GraphicsDevice
{
public:
    GraphicsDeviceGL();
    virtual ~GraphicsDeviceGL();
    
public:
    virtual std::shared_ptr<RenderBufferCapture> CaptureRenderBuffer();
    
    virtual void SetClearColor(glm::vec4 color);
    virtual void ClearBuffers();
    
    virtual void ResetState();
    
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
    virtual Texture* GetBoundTexture(int textureUnit);
    virtual Texture* BindTexture(int textureUnit, Texture* texture, bool force=false);
    
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
    DeviceState _DesiredState;
    
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
    
    friend struct DeviceState;
};

}

#endif
