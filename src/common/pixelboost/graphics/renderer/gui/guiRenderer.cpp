#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"

using namespace pb;

GuiRenderer* GuiRenderer::_Instance = 0;

GuiRenderable::GuiRenderable(Uid entityUid)
    : pb::Renderable(entityUid)
{
    
}

GuiRenderable::~GuiRenderable()
{
    
}

Uid GuiRenderable::GetType()
{
    return GuiRenderable::GetStaticType();
}

Uid GuiRenderable::GetStaticType()
{
    return TypeHash("pb::GwenRenderable");
}

void GuiRenderable::CalculateBounds()
{
    SetBounds(BoundingSphere());
}

void GuiRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(glm::mat4x4());
}

Shader* GuiRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/shaders/pb_textured.shc");
}

void GuiRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;
    DirtyBounds();
    DirtyWorldMatrix();
}

GuiRenderer::GuiRenderer()
{
    _Instance = this;
    
    _MaxQuads = 4000;
    
    _IndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, 6*_MaxQuads);
    _VertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P3_UV, 4*_MaxQuads);
    _VertexBuffer->Lock();
    Vertex_P3_UV* vertices = static_cast<Vertex_P3_UV*>(_VertexBuffer->GetData());
    for (int i=0; i<4*_MaxQuads; i++)
    {
        vertices[i].position[2] = 0;
    }
    _VertexBuffer->Unlock();
    
    _IndexBuffer->Lock();
    unsigned short* indicies = _IndexBuffer->GetData();
    for (int i=0; i<_MaxQuads; i++)
    {
        int offset = i * 4;
        indicies[0] = offset + 0;
        indicies[1] = offset + 1;
        indicies[2] = offset + 2;
        indicies[3] = offset + 0;
        indicies[4] = offset + 2;
        indicies[5] = offset + 3;
        indicies += 6;
    }
    _IndexBuffer->Unlock();
    
    Renderer::Instance()->SetHandler(GuiRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/shaders/pb_textured.shc");
}

GuiRenderer::~GuiRenderer()
{
    _Instance = 0;
    
    Renderer::Instance()->GetShaderManager()->UnloadShader("/shaders/pb_textured.shc");
}

GuiRenderer* GuiRenderer::Instance()
{
    return _Instance;
}

void GuiRenderer::Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass)
{
    shaderPass->GetShaderProgram()->SetUniform("_DiffuseColor", glm::vec4(1,1,1,1));
    shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
    for (int i=0; i<count; i++)
    {
        //GuiRenderable* renderable = static_cast<GuiRenderable*>(renderables[i]);
    }
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
}
