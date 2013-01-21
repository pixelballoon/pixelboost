#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
#include "pixelboost/logic/component/graphics/buffer.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

BufferComponent::BufferComponent(Entity* entity, IndexBuffer* indexBuffer, VertexBuffer* vertexBuffer, Texture* texture, int numElements)
    : Component(entity)
{
    _Renderable = new BufferRenderable(GetParentUid(), indexBuffer, vertexBuffer, texture, numElements);
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
}

BufferComponent::~BufferComponent()
{
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid BufferComponent::GetType()
{
    return BufferComponent::GetStaticType();
}

Uid BufferComponent::GetStaticType()
{
    return TypeHash("pb::BufferComponent");
}

void BufferComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void BufferComponent::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    _Renderable->SetIndexBuffer(indexBuffer);
}

IndexBuffer* BufferComponent::GetIndexBuffer()
{
    return _Renderable->GetIndexBuffer();
}

void BufferComponent::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
    _Renderable->SetVertexBuffer(vertexBuffer);
}

VertexBuffer* BufferComponent::GetVertexBuffer()
{
    return _Renderable->GetVertexBuffer();
}

void BufferComponent::SetTexture(Texture* texture)
{
    _Renderable->SetTexture(texture);
}

Texture* BufferComponent::GetTexture()
{
    return _Renderable->GetTexture();
}

void BufferComponent::SetShader(Shader* shader)
{
    _Renderable->SetShader(shader);
}

Shader* BufferComponent::GetShader()
{
    return _Renderable->GetShader();
}

void BufferComponent::SetNumElements(int numElements)
{
    _Renderable->SetNumElements(numElements);
}

int BufferComponent::GetNumElements()
{
    return _Renderable->GetNumElements();
}
