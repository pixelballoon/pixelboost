#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"
#include "pixelboost/graphics/resources/materialResource.h"
#include "pixelboost/logic/component/graphics/trail.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::TrailComponent)

TrailComponent::TrailComponent(Entity* parent)
    : Component(parent)
{
    _Length = 0;
    _MaxElements = 5000;
    _MinDistance = 0.3f;
    
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, _MaxElements*6);
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P3_C4_UV, _MaxElements*4);

    _Renderable = new BufferRenderable();
    _Renderable->SetIndexBuffer(_IndexBuffer);
    _Renderable->SetVertexBuffer(_VertexBuffer);
    
    _IndexBuffer->Lock();
    unsigned short* index = _IndexBuffer->GetData();
    for (int i=0; i<_MaxElements; i++)
    {
        index[0] = (i*4);
        index[1] = (i*4)+1;
        index[2] = (i*4)+2;
        index[3] = (i*4)+1;
        index[4] = (i*4)+2;
        index[5] = (i*4)+3;
        index += 6;
    }
    _IndexBuffer->Unlock();
    
    _VertexBuffer->Lock();
    _VertexBuffer->Unlock(0);
    
    GetScene()->GetSystemByType<RenderSystem>()->AddItem(_Renderable);
    
    GetEntity()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &TrailComponent::OnTransformChanged));
}

TrailComponent::~TrailComponent()
{
    GetEntity()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &TrailComponent::OnTransformChanged));
    
    if (_Material)
    {
        _Material->SignalResourceLoaded.Disconnect(this, &TrailComponent::OnResourceLoaded);
        _Material->SignalResourceUnloading.Disconnect(this, &TrailComponent::OnResourceUnloading);
    }
    
    GetScene()->GetSystemByType<RenderSystem>()->RemoveItem(_Renderable);
}

void TrailComponent::SetMinDistance(float minDistance)
{
    _MinDistance = minDistance;
}

void TrailComponent::SetMaterial(const std::string& resource, const std::string& pool)
{
    if (_Material)
    {
        _Material->SignalResourceLoaded.Disconnect(this, &TrailComponent::OnResourceLoaded);
        _Material->SignalResourceUnloading.Disconnect(this, &TrailComponent::OnResourceUnloading);
    }
    
    _Material = ResourceManager::Instance()->GetPool(pool)->GetResource<MaterialResource>(resource);
    _Material->SignalResourceLoaded.Connect(this, &TrailComponent::OnResourceLoaded);
    _Material->SignalResourceUnloading.Connect(this, &TrailComponent::OnResourceUnloading);
    
    if (_Material->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_Material.get(), false);
    }
}

std::shared_ptr<MaterialResource> TrailComponent::GetMaterial()
{
    return _Material;
}

BufferRenderable* TrailComponent::GetRenderable()
{
    return _Renderable;
}

void TrailComponent::OnTransformChanged(const pb::Message& message)
{
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();
    
    glm::vec4 position = transform->GetMatrix() * glm::vec4(0,0,0,1);
    
    glm::vec2 point(position.x, position.y);
    
    if (_Points.size() && glm::distance(point, _Points.back()) < _MinDistance)
        return;
    
    _Points.push_back(point);
    
    _VertexBuffer->Lock();
    pb::Vertex_P3_C4_UV* vertices = static_cast<pb::Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    if (_Points.size() > 2 && _Points.size() < _MaxElements)
    {
        glm::vec2 point[3];
        
        int numPoints = _Points.size()-1;
        point[0] = _Points[numPoints-2];
        point[1] = _Points[numPoints-1];
        point[2] = _Points[numPoints];
        
        float prevLength = _Length;
        _Length += glm::distance(point[2], point[1]) / 24.f;
        
        float width = 0.1f;
        float angle = glm::atan(point[2].y-point[1].y, point[2].x-point[1].x) + glm::radians(90.f);
        
        vertices[(numPoints*4)-2].position[0] = point[1].x + cos(angle)*width;
        vertices[(numPoints*4)-2].position[1] = point[1].y + sin(angle)*width;
        vertices[(numPoints*4)-1].position[0] = point[1].x + cos(angle+M_PI)*width;
        vertices[(numPoints*4)-1].position[1] = point[1].y + sin(angle+M_PI)*width;
        
        vertices[(numPoints*4)+0].position[0] = point[1].x + cos(angle)*width;
        vertices[(numPoints*4)+0].position[1] = point[1].y + sin(angle)*width;
        vertices[(numPoints*4)+0].position[2] = 0;
        vertices[(numPoints*4)+0].uv[0] = 0;
        vertices[(numPoints*4)+0].uv[1] = prevLength;
        vertices[(numPoints*4)+0].color[0] = 1.f;
        vertices[(numPoints*4)+0].color[1] = 1.f;
        vertices[(numPoints*4)+0].color[2] = 1.f;
        vertices[(numPoints*4)+0].color[3] = 1.f;
        
        vertices[(numPoints*4)+1].position[0] = point[1].x + cos(angle+M_PI)*width;
        vertices[(numPoints*4)+1].position[1] = point[1].y + sin(angle+M_PI)*width;
        vertices[(numPoints*4)+1].position[2] = 0;
        vertices[(numPoints*4)+1].uv[0] = 1;
        vertices[(numPoints*4)+1].uv[1] = prevLength;
        vertices[(numPoints*4)+1].color[0] = 1.f;
        vertices[(numPoints*4)+1].color[1] = 1.f;
        vertices[(numPoints*4)+1].color[2] = 1.f;
        vertices[(numPoints*4)+1].color[3] = 1.f;
        
        vertices[(numPoints*4)+2].position[0] = point[2].x + cos(angle)*width;
        vertices[(numPoints*4)+2].position[1] = point[2].y + sin(angle)*width;
        vertices[(numPoints*4)+2].position[2] = 0;
        vertices[(numPoints*4)+2].uv[0] = 0;
        vertices[(numPoints*4)+2].uv[1] = _Length;
        vertices[(numPoints*4)+2].color[0] = 1.f;
        vertices[(numPoints*4)+2].color[1] = 1.f;
        vertices[(numPoints*4)+2].color[2] = 1.f;
        vertices[(numPoints*4)+2].color[3] = 1.f;
        
        vertices[(numPoints*4)+3].position[0] = point[2].x + cos(angle+M_PI)*width;
        vertices[(numPoints*4)+3].position[1] = point[2].y + sin(angle+M_PI)*width;
        vertices[(numPoints*4)+3].position[2] = 0;
        vertices[(numPoints*4)+3].uv[0] = 1;
        vertices[(numPoints*4)+3].uv[1] = _Length;
        vertices[(numPoints*4)+3].color[0] = 1.f;
        vertices[(numPoints*4)+3].color[1] = 1.f;
        vertices[(numPoints*4)+3].color[2] = 1.f;
        vertices[(numPoints*4)+3].color[3] = 1.f;
        
        _Renderable->SetNumElements(_Points.size()*6);
    } else {
        glm::vec2 point = _Points.size() ? _Points[0] : glm::vec2(0,0);
        for (int i=0; i<8; i++)
        {
            vertices[i].position[0] = point.x;
            vertices[i].position[1] = point.y;
            vertices[i].position[2] = 0;
        }
    }
    
    _VertexBuffer->Unlock(_Points.size()*4);
}
    
void TrailComponent::OnResourceLoaded(Resource* resource, bool error)
{
    if (error)
        return;
    
    _Renderable->SetMaterial(_Material->GetMaterial());
}
    
void TrailComponent::OnResourceUnloading(Resource* resource)
{
    _Renderable->SetMaterial(0);
}
