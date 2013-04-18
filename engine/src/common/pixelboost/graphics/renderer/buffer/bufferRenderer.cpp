#include "pixelboost/debug/assert.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/material/material.h"
#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/resource/resourceManager.h"

using namespace pb;

BufferRenderer* BufferRenderer::_Instance = 0;

BufferRenderable::BufferRenderable()
    : _IndexBuffer(0)
    , _VertexBuffer(0)
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
    
    return ResourceManager::Instance()->GetPool("default")->GetResource<ShaderResource>("/shaders/pb_textured.shc")->GetShader();
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
    PbAssert(!_Instance);
    
    _Instance = this;
    
    Renderer::Instance()->SetHandler(BufferRenderable::GetStaticType(), this);
}
    
BufferRenderer::~BufferRenderer()
{
    _Instance = 0;
}

BufferRenderer* BufferRenderer::Instance()
{
    return _Instance;
}

void BufferRenderer::Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    Material* material = renderables[0]->GetMaterial();
    
    if (!material)
        return;
    
    ShaderPass* shaderPass = material->Bind(renderScheme, 0, projectionMatrix, viewMatrix);
    
    for (int i=0; i<count; i++)
    {
        BufferRenderable& renderable = *static_cast<BufferRenderable*>(renderables[i]);
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable.GetModelViewMatrix());
        
        GraphicsDevice::Instance()->BindIndexBuffer(renderable._IndexBuffer);
        GraphicsDevice::Instance()->BindVertexBuffer(renderable._VertexBuffer);
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, renderable._NumElements);
    }
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
}
