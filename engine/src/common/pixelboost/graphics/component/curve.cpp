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
    _LengthScale = 1.f;
    _MinDistance = 1.f;
    _IndexBuffer = 0;
    _VertexBuffer = 0;

    GetRenderable()->SetIndexBuffer(_IndexBuffer);
    GetRenderable()->SetVertexBuffer(_VertexBuffer);
    GetRenderable()->SetNumElements(0);
    
    GetEntity()->RegisterMessageHandler<SetColorMessage>(MessageHandler(this, &CurveComponent::OnSetColor));
//    GetEntity()->RegisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &CurveComponent::OnDebugRender));
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

void CurveComponent::ResetCurves()
{
    _Curves.clear();
}

void CurveComponent::AddCurve(const HermiteCurve2D* curve, bool skipRefresh)
{
    _Curves.insert(curve);
    
    if (!skipRefresh)
    {
        Refresh();
    }
}

void CurveComponent::RemoveCurve(const HermiteCurve2D* curve, bool skipRefresh)
{
    _Curves.erase(curve);
    
    if (!skipRefresh)
    {
        Refresh();
    }
}

const std::set<const HermiteCurve2D*>& CurveComponent::GetCurves()
{
    return _Curves;
}

void CurveComponent::Refresh()
{
    std::vector<int> sizes;
    int totalSize = 0;
    
    for (const auto& curve : _Curves)
    {
        int size = curve->GetArcLength() / _MinDistance;
        sizes.push_back(size);
        totalSize += size;
    }
    
    if (_IndexBuffer && _IndexBuffer->GetCurrentSize() != (totalSize * 6))
    {
        GetRenderable()->SetIndexBuffer(0);
        GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
        _IndexBuffer = 0;
    }
    
    if (_VertexBuffer && _VertexBuffer->GetCurrentSize() != (totalSize * 2))
    {
        GetRenderable()->SetVertexBuffer(0);
        GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
        _VertexBuffer = 0;
    }
    
    if (_Curves.size())
    {
        if (!_IndexBuffer)
        {
            _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, totalSize*6);
        }
        
        if (!_VertexBuffer)
        {
            _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatStatic, pb::kVertexFormat_P3_C4_UV, totalSize*2);
        }
        
        _IndexBuffer->Lock();
        unsigned short* index = _IndexBuffer->GetData();
        int vertexIndex = 0;
        for (const auto& size : sizes)
        {
            for (int i=0; i<size-1; i++)
            {
                index[0] = vertexIndex;
                index[1] = vertexIndex+1;
                index[2] = vertexIndex+3;
                index[3] = vertexIndex;
                index[4] = vertexIndex+2;
                index[5] = vertexIndex+3;
                index += 6;
                vertexIndex += 2;
            }
            vertexIndex += 2;
        }
        _IndexBuffer->Unlock();
                
        _VertexBuffer->Lock();
        pb::Vertex_P3_C4_UV* vertices = static_cast<pb::Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
        for (const auto& curve : _Curves)
        {
            float t = 0;
            int size = curve->GetArcLength() / _MinDistance;
            
            for (int i=0; i<size; i++)
            {
                glm::vec2 point = curve->EvaluateParam(t);
                glm::vec2 pointB = curve->EvaluateParam(t + 0.1f);
                
                t += _MinDistance;
                
                float angle = glm::atan(pointB.y-point.y, pointB.x-point.x) + glm::radians(90.f);
                
                for (int v=0; v<2; v++)
                {
                    vertices->position[0] = point.x + cos(v == 0 ? angle-glm::radians(180.f) : angle)*_Width;
                    vertices->position[1] = point.y + sin(v == 0 ? angle-glm::radians(180.f) : angle)*_Width;
                    vertices->position[2] = 0;
                    vertices->uv[0] = (v == 0 ? 0 : 1);
                    vertices->uv[1] = t / _LengthScale;
                    vertices->color[0] = _Tint.r;
                    vertices->color[1] = _Tint.g;
                    vertices->color[2] = _Tint.b;
                    vertices->color[3] = _Tint.a;
                    vertices++;
                }
            }
        }
        _VertexBuffer->Unlock();
        
        GetRenderable()->SetIndexBuffer(_IndexBuffer);
        GetRenderable()->SetVertexBuffer(_VertexBuffer);
        GetRenderable()->SetNumElements((totalSize-sizes.size())*6);
    } else {
        GetRenderable()->SetNumElements(0);
    }
}

void CurveComponent::OnDebugRender(const Message& message)
{
    auto debugRenderMessage = message.As<DebugRenderMessage>();
    
    for (const auto& curve : _Curves)
    {
        int size = curve->GetArcLength() / _MinDistance;
        float t = 0;
        
        glm::vec2 prevPoint = curve->EvaluateParam(t);
    
        for (int i=0; i<=size; i++)
        {
            glm::vec2 point = curve->EvaluateParam(t);
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
    
    CurveComponent::ResetCurves();
    
    if (error)
    {
        return;
    }
    
    auto svgResource = static_cast<SvgResource*>(resource);
    
    bool curveFound = false;
    
    for (const auto& group : svgResource->GetGroups())
    {
        if (group.first == _CurveName)
        {
            for (const auto& path : group.second.Paths)
            {
                curveFound = true;
                CurveComponent::AddCurve(&path.Curve, true);
            }
            break;
        }
    }
    
    if (!curveFound)
    {
        PbLogWarn("graphics.curve", "Curve %s not found on resource %s", _CurveName.c_str(), resource->GetFilename().c_str());
    }
    
    CurveComponent::Refresh();

}

void CurveComponentSVG::OnResourceUnloading(Resource* resource)
{
    CurveComponent::ResetCurves();
}
