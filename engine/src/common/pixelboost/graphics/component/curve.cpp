#include "glm/gtc/random.hpp"

#include "pixelboost/data/resources/svgResource.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/component/curve.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/message/color.h"
#include "pixelboost/logic/component/graphics/buffer.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/maths/hermiteCurve.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::CurveComponent)

CurveComponent::CurveComponent(Entity* parent)
    : RenderableComponent<BufferRenderable>(parent)
{
    _Curve = 0;
    _LengthScale = 1.f;
    _MinDistance = 1.f;
    _IndexBuffer = 0;
    _VertexBuffer = 0;

    GetRenderable()->SetIndexBuffer(_IndexBuffer);
    GetRenderable()->SetVertexBuffer(_VertexBuffer);
    GetRenderable()->SetNumElements(0);
    
    GetEntity()->RegisterMessageHandler<SetColorMessage>(MessageHandler(this, &CurveComponent::OnSetColor));
    //GetEntity()->RegisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &CurveComponent::OnDebugRender));
}

CurveComponent::~CurveComponent()
{
    GetEntity()->UnregisterMessageHandler<SetColorMessage>(MessageHandler(this, &CurveComponent::OnSetColor));
    GetEntity()->UnregisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &CurveComponent::OnDebugRender));
}

void CurveComponent::SetMinDistance(float minDistance)
{
	_MinDistance = minDistance;
}

float CurveComponent::GetMinDistance()
{
	return _MinDistance;
}

void CurveComponent::SetLengthScale(float lengthScale)
{
    _LengthScale = lengthScale;
}

float CurveComponent::GetLengthScale()
{
    return _LengthScale;
}

void CurveComponent::SetWidth(float width)
{
    _Width = width;
}

float CurveComponent::GetWidth()
{
    return _Width;
}

void CurveComponent::SetTint(const glm::vec4& tint)
{
    _Tint = tint;
    
    Refresh();
}

const glm::vec4& CurveComponent::GetTint()
{
    return _Tint;
}

void CurveComponent::SetCurve(const HermiteCurve2D* curve, bool skipRefresh)
{
    _Curve = curve;
    
    if (!skipRefresh)
    {
        Refresh();
    }
}

const HermiteCurve2D* CurveComponent::GetCurve()
{
    return _Curve;
}

void CurveComponent::Refresh()
{
    float t = 0;
    int size = _Curve ? (_Curve->GetArcLength() / _MinDistance) : 0;
    
    if (!_Curve || (_IndexBuffer && _IndexBuffer->GetCurrentSize() != (size * 6 - 4)))
    {
        GetRenderable()->SetIndexBuffer(0);
        GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
        _IndexBuffer = 0;
    }
    
    if (!_Curve || (_VertexBuffer && _VertexBuffer->GetCurrentSize() != (size * 2)))
    {
        GetRenderable()->SetVertexBuffer(0);
        GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
        _VertexBuffer = 0;
    }
    
    if (_Curve)
    {
        if (!_IndexBuffer)
        {
            _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, size*6 - 4);
        }
        
        if (!_VertexBuffer)
        {
            _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatStatic, pb::kVertexFormat_P3_C4_UV, size*2);
        }
        
        _IndexBuffer->Lock();
        unsigned short* index = _IndexBuffer->GetData();
        for (int i=0; i<size*2; i+=2)
        {
            index[0] = i;
            index[1] = i+1;
            index[2] = i+3;
            index[3] = i;
            index[4] = i+2;
            index[5] = i+3;
            index += 6;
        }
        _IndexBuffer->Unlock();
                
        _VertexBuffer->Lock();
        pb::Vertex_P3_C4_UV* vertices = static_cast<pb::Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
        for (int i=0; i<size; i++)
        {
            glm::vec2 point = _Curve->EvaluateParam(t);
            glm::vec2 pointB = _Curve->EvaluateParam(t + 0.1f);
            
            t += _MinDistance;
            
            float angle = glm::atan(pointB.y-point.y, pointB.x-point.x) + glm::radians(90.f);
            
            for (int v=0; v<2; v++)
            {
                vertices[(i*2)+v].position[0] = point.x + cos(v == 0 ? angle-glm::radians(180.f) : angle)*_Width;
                vertices[(i*2)+v].position[1] = point.y + sin(v == 0 ? angle-glm::radians(180.f) : angle)*_Width;
                vertices[(i*2)+v].position[2] = 0;
                vertices[(i*2)+v].uv[0] = (v == 0 ? 0 : 1);
                vertices[(i*2)+v].uv[1] = t / _LengthScale;
                vertices[(i*2)+v].color[0] = _Tint.r;
                vertices[(i*2)+v].color[1] = _Tint.g;
                vertices[(i*2)+v].color[2] = _Tint.b;
                vertices[(i*2)+v].color[3] = _Tint.a;
            }
        }
        _VertexBuffer->Unlock();
        
        GetRenderable()->SetIndexBuffer(_IndexBuffer);
        GetRenderable()->SetVertexBuffer(_VertexBuffer);
        GetRenderable()->SetNumElements(size*6 - 4);
    }
}

void CurveComponent::OnDebugRender(const Message& message)
{
    auto debugRenderMessage = message.As<DebugRenderMessage>();
    
    if (_Curve)
    {
        int size = _Curve->GetArcLength() / _MinDistance;
        float t = 0;
        
        glm::vec2 prevPoint = _Curve->EvaluateParam(t);
    
        for (int i=0; i<size; i++)
        {
            glm::vec2 point = _Curve->EvaluateParam(t);
            t += _MinDistance;
            
            debugRenderMessage.GetDebugRenderSystem()->AddLine(kRenderPassScene, 1, glm::vec3(prevPoint, 0), glm::vec3(point, 0));
            
            prevPoint = point;
        }
    }
}

void CurveComponent::OnSetColor(const Message& message)
{
    auto setColorMessage = message.As<SetColorMessage>();
    
    SetTint(setColorMessage.GetColor());
}

PB_DEFINE_COMPONENT(pb::CurveComponentSVG)

CurveComponentSVG::CurveComponentSVG(Entity* parent)
    : CurveComponent(parent)
{
    
}

CurveComponentSVG::~CurveComponentSVG()
{
    GetEntity()->UnregisterMessageHandler(MessageHandler(this, &CurveComponentSVG::OnUpdate));
}
    
void CurveComponentSVG::SetCurve(const std::string& resource, const std::string& curve)
{
    _CurveName = curve;
    
    _Resource = ResourceManager::Instance()->GetPool("default")->GetResource<SvgResource>(resource);
    _Resource->SignalResourceLoaded.Connect(this, &CurveComponentSVG::OnResourceLoaded);
    _Resource->SignalResourceUnloading.Connect(this, &CurveComponentSVG::OnResourceUnloading);
    if (_Resource->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_Resource.get(), false);
    }
}

void CurveComponentSVG::OnUpdate(const Message& message)
{
    GetEntity()->UnregisterMessageHandler(MessageHandler(this, &CurveComponentSVG::OnUpdate));
    
    Refresh();
}

void CurveComponentSVG::OnResourceLoaded(Resource* resource, bool error)
{
    GetEntity()->RegisterMessageHandler(MessageHandler(this, &CurveComponentSVG::OnUpdate));
    
    if (error)
    {
        CurveComponent::SetCurve(0, true);
        return;
    }
    
    auto svgResource = static_cast<SvgResource*>(resource);
    
    for (const auto& group : svgResource->GetGroups())
    {
        for (const auto& path : group.second.Paths)
        {
            CurveComponent::SetCurve(&path.Curve, true);
        }
    }

}

void CurveComponentSVG::OnResourceUnloading(Resource* resource)
{
    CurveComponent::SetCurve(0);
}
