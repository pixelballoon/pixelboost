#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/message/color.h"
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
    _MinDistance = 1.f;
    _Width = 10.f;
    _Tint = glm::vec4(1,1,1,1);
    _IndexBuffer = 0;
    _VertexBuffer = 0;
    
    _Renderable = new BufferRenderable();
    GetScene()->GetSystemByType<RenderSystem>()->AddItem(_Renderable);
    
    SetMaxSegments(32);
    
    GetEntity()->RegisterMessageHandler<SetColorMessage>(MessageHandler(this, &TrailComponent::OnSetColor));
    GetEntity()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &TrailComponent::OnTransformChanged));
}

TrailComponent::~TrailComponent()
{
    GetEntity()->UnregisterMessageHandler<SetColorMessage>(MessageHandler(this, &TrailComponent::OnSetColor));
    GetEntity()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &TrailComponent::OnTransformChanged));
    
    if (_Material)
    {
        _Material->SignalResourceLoaded.Disconnect(this, &TrailComponent::OnResourceLoaded);
        _Material->SignalResourceUnloading.Disconnect(this, &TrailComponent::OnResourceUnloading);
    }
    
    GetScene()->GetSystemByType<RenderSystem>()->RemoveItem(_Renderable);
    
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    
    delete _Renderable;
}

void TrailComponent::SetMaxSegments(int maxSegments)
{
    if (maxSegments == _MaxSegments)
    {
        return;
    }
    
    for (int i=0; i<10; i++)
    
    {
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    
    _MaxSegments = maxSegments;
    
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, _MaxSegments*6);
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P3_C4_UV, _MaxSegments*4);
    
    _Renderable->SetIndexBuffer(_IndexBuffer);
    _Renderable->SetVertexBuffer(_VertexBuffer);
    
    _IndexBuffer->Lock();
    unsigned short* index = _IndexBuffer->GetData();
    for (int i=0; i<_MaxSegments; i++)
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
    }
}

int TrailComponent::GetMaxSegments()
{
    return _MaxSegments;
}

void TrailComponent::SetMinDistance(float minDistance)
{
    _MinDistance = minDistance;
}

float TrailComponent::GetMinDistance()
{
    return _MinDistance;
}

void TrailComponent::SetWidth(float width)
{
    _Width = width;
}

float TrailComponent::GetWidth()
{
    return _Width;
}

void TrailComponent::SetLengthScale(float lengthScale)
{
    _LengthScale = lengthScale;
}

float TrailComponent::GetLengthScale()
{
    return _LengthScale;
}

void TrailComponent::SetTint(const glm::vec4& tint)
{
    _Tint = tint;
    
    int numVertices = _VertexBuffer->GetCurrentSize();
    if (numVertices)
    {
        _VertexBuffer->Lock();
        pb::Vertex_P3_C4_UV* vertices = static_cast<pb::Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
        for (int i=0; i<numVertices; i++)
        {
            vertices[i].color[0] = _Tint.r;
            vertices[i].color[1] = _Tint.g;
            vertices[i].color[2] = _Tint.b;
            vertices[i].color[3] = _Tint.a;
        }
        _VertexBuffer->Unlock(numVertices);
    }
}

const glm::vec4& TrailComponent::GetTint()
{
    return _Tint;
}

void TrailComponent::SetMaterial(const std::string& resource)
{
    if (_Material)
    {
        _Material->SignalResourceLoaded.Disconnect(this, &TrailComponent::OnResourceLoaded);
        _Material->SignalResourceUnloading.Disconnect(this, &TrailComponent::OnResourceUnloading);
    }
    
    _Material = ResourceManager::Instance()->GetPool("default")->GetResource<MaterialResource>(resource);
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

void TrailComponent::OnSetColor(const Message& message)
{
    auto colorMessage = message.As<SetColorMessage>();
    
    SetTint(colorMessage.GetColor());
}

void TrailComponent::OnTransformChanged(const Message& message)
{
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();
    
    glm::vec4 position = transform->GetMatrix() * glm::vec4(0,0,0,1);
    
    glm::vec2 point(position.x, position.y);
    
    if (_Points.size() && (glm::distance(point, _Points.back()) < _MinDistance || _Points.size() >= _MaxSegments))
        return;
    
    _Points.push_back(point);
    
    _VertexBuffer->Lock();
    pb::Vertex_P3_C4_UV* vertices = static_cast<pb::Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    if (_Points.size() > 2 && _Points.size() < _MaxSegments)
    {
        glm::vec2 point[3];
        
        int numPoints = _Points.size()-1;
        point[0] = _Points[numPoints-2];
        point[1] = _Points[numPoints-1];
        point[2] = _Points[numPoints];
        
        float prevLength = _Length;
        _Length += glm::distance(point[2], point[1]) / _LengthScale;
        
        float angle = glm::atan(point[2].y-point[1].y, point[2].x-point[1].x) + glm::radians(90.f);
        
        vertices[(numPoints*4)-2].position[0] = point[1].x + cos(angle)*_Width;
        vertices[(numPoints*4)-2].position[1] = point[1].y + sin(angle)*_Width;
        vertices[(numPoints*4)-1].position[0] = point[1].x + cos(angle+M_PI)*_Width;
        vertices[(numPoints*4)-1].position[1] = point[1].y + sin(angle+M_PI)*_Width;
        
        vertices[(numPoints*4)+0].position[0] = point[1].x + cos(angle)*_Width;
        vertices[(numPoints*4)+0].position[1] = point[1].y + sin(angle)*_Width;
        vertices[(numPoints*4)+0].position[2] = 0;
        vertices[(numPoints*4)+0].uv[0] = 0;
        vertices[(numPoints*4)+0].uv[1] = prevLength;
        vertices[(numPoints*4)+0].color[0] = _Tint.r;
        vertices[(numPoints*4)+0].color[1] = _Tint.g;
        vertices[(numPoints*4)+0].color[2] = _Tint.b;
        vertices[(numPoints*4)+0].color[3] = _Tint.a;
        
        vertices[(numPoints*4)+1].position[0] = point[1].x + cos(angle+M_PI)*_Width;
        vertices[(numPoints*4)+1].position[1] = point[1].y + sin(angle+M_PI)*_Width;
        vertices[(numPoints*4)+1].position[2] = 0;
        vertices[(numPoints*4)+1].uv[0] = 1;
        vertices[(numPoints*4)+1].uv[1] = prevLength;
        vertices[(numPoints*4)+1].color[0] = _Tint.r;
        vertices[(numPoints*4)+1].color[1] = _Tint.g;
        vertices[(numPoints*4)+1].color[2] = _Tint.b;
        vertices[(numPoints*4)+1].color[3] = _Tint.a;
        
        vertices[(numPoints*4)+2].position[0] = point[2].x + cos(angle)*_Width;
        vertices[(numPoints*4)+2].position[1] = point[2].y + sin(angle)*_Width;
        vertices[(numPoints*4)+2].position[2] = 0;
        vertices[(numPoints*4)+2].uv[0] = 0;
        vertices[(numPoints*4)+2].uv[1] = _Length;
        vertices[(numPoints*4)+2].color[0] = _Tint.r;
        vertices[(numPoints*4)+2].color[1] = _Tint.g;
        vertices[(numPoints*4)+2].color[2] = _Tint.b;
        vertices[(numPoints*4)+2].color[3] = _Tint.a;
        
        vertices[(numPoints*4)+3].position[0] = point[2].x + cos(angle+M_PI)*_Width;
        vertices[(numPoints*4)+3].position[1] = point[2].y + sin(angle+M_PI)*_Width;
        vertices[(numPoints*4)+3].position[2] = 0;
        vertices[(numPoints*4)+3].uv[0] = 1;
        vertices[(numPoints*4)+3].uv[1] = _Length;
        vertices[(numPoints*4)+3].color[0] = _Tint.r;
        vertices[(numPoints*4)+3].color[1] = _Tint.g;
        vertices[(numPoints*4)+3].color[2] = _Tint.b;
        vertices[(numPoints*4)+3].color[3] = _Tint.a;
        
        _Renderable->SetNumElements(_Points.size()*6);
    } else {
        glm::vec2 point = _Points.size() ? _Points[0] : glm::vec2(0,0);
        for (int i=0; i<8; i++)
        {
            vertices[i].position[0] = point.x;
            vertices[i].position[1] = point.y;
            vertices[i].position[2] = 0;
            vertices[i].color[0] = 0;
            vertices[i].color[1] = 0;
            vertices[i].color[2] = 0;
            vertices[i].color[3] = 0;
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
