#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
#include "pixelboost/logic/component/graphics/buffer.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::BufferComponent)

BufferComponent::BufferComponent(Entity* parent)
    : RenderableComponent<BufferRenderable>(parent)
{
    
}

BufferComponent::~BufferComponent()
{
    
}

void BufferComponent::SetLayer(int layer)
{
    GetRenderable()->SetLayer(layer);
}

void BufferComponent::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    GetRenderable()->SetIndexBuffer(indexBuffer);
}

IndexBuffer* BufferComponent::GetIndexBuffer()
{
    return GetRenderable()->GetIndexBuffer();
}

void BufferComponent::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
    GetRenderable()->SetVertexBuffer(vertexBuffer);
}

VertexBuffer* BufferComponent::GetVertexBuffer()
{
    return GetRenderable()->GetVertexBuffer();
}

void BufferComponent::SetTexture(Texture* texture)
{
    GetRenderable()->SetTexture(texture);
}

Texture* BufferComponent::GetTexture()
{
    return GetRenderable()->GetTexture();
}

void BufferComponent::SetShader(Shader* shader)
{
    GetRenderable()->SetShader(shader);
}

Shader* BufferComponent::GetShader()
{
    return GetRenderable()->GetShader();
}

void BufferComponent::SetNumElements(int numElements)
{
    GetRenderable()->SetNumElements(numElements);
}

int BufferComponent::GetNumElements()
{
    return GetRenderable()->GetNumElements();
}
