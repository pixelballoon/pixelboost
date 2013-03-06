#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"

using namespace pb;

BufferRenderable::BufferRenderable(Uid entityId)
    : Renderable(entityId)
    , _IndexBuffer(0)
    , _VertexBuffer(0)
    , _Texture(0)
    , _NumElements(0)
{
    
}

BufferRenderable::~BufferRenderable()
{

}

Uid BufferRenderable::GetType()
{
    return BufferRenderable::GetStaticType();
}

Uid BufferRenderable::GetStaticType()
{
    return TypeHash("pb::BufferRenderable");
}

void BufferRenderable::CalculateBounds()
{
    SetBounds(_Bounds);
}

void BufferRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(_Transform);
}

Shader* BufferRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/shaders/pb_textured.shc");
}

void BufferRenderable::SetBounds(BoundingSphere bounds)
{
    _Bounds = bounds;
}

void BufferRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;
    DirtyBounds();
    DirtyWorldMatrix();
}

void BufferRenderable::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    _IndexBuffer = indexBuffer;
}

IndexBuffer* BufferRenderable::GetIndexBuffer()
{
    return _IndexBuffer;
}

void BufferRenderable::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
    _VertexBuffer = vertexBuffer;
}

VertexBuffer* BufferRenderable::GetVertexBuffer()
{
    return _VertexBuffer;
}

void BufferRenderable::SetTexture(Texture* texture)
{
    _Texture = texture;
}

Texture* BufferRenderable::GetTexture()
{
    return _Texture;
}

void BufferRenderable::SetNumElements(int numElements)
{
    _NumElements = numElements;
}

int BufferRenderable::GetNumElements()
{
    return _NumElements;
}
    
BufferRenderer::BufferRenderer()
{
    Renderer::Instance()->SetHandler(BufferRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/shaders/pb_textured.shc");
}
    
BufferRenderer::~BufferRenderer()
{
    
}

void BufferRenderer::Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
    
    shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
    
    for (int i=0; i<count; i++)
    {
        BufferRenderable& renderable = *static_cast<BufferRenderable*>(renderables[i]);
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable.GetModelViewMatrix());
        
        GraphicsDevice::Instance()->BindTexture(renderable._Texture);
        GraphicsDevice::Instance()->BindIndexBuffer(renderable._IndexBuffer);
        GraphicsDevice::Instance()->BindVertexBuffer(renderable._VertexBuffer);
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, renderable._NumElements);
    }
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
}

#endif
