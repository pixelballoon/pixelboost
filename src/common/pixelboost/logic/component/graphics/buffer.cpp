#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
#include "pixelboost/logic/component/graphics/buffer.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(BufferComponent)

BufferComponent::BufferComponent(Entity* entity, IndexBuffer* indexBuffer, VertexBuffer* vertexBuffer, Texture* texture, int numElements)
    : Component(entity)
{
    _Renderable = new BufferRenderable(GetParentUid(), indexBuffer, vertexBuffer, texture, numElements);
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &BufferComponent::OnTransformChanged));
    
    UpdateTransform();
}

BufferComponent::~BufferComponent()
{
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &BufferComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
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

void BufferComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void BufferComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void BufferComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    } else {
        _Renderable->SetTransform(_LocalTransform);
    }
}
