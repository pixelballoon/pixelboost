#include "glm/gtc/random.hpp"

#include "pixelboost/data/resources/svgResource.h"
#include "pixelboost/graphics/component/curve.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
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
    _MinDistance = 1.f;
    _IndexBuffer = 0;
    _VertexBuffer = 0;

    GetRenderable()->SetIndexBuffer(_IndexBuffer);
    GetRenderable()->SetVertexBuffer(_VertexBuffer);
    GetRenderable()->SetNumElements(0);
    
    //GetEntity()->RegisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &CurveComponent::OnDebugRender));
}

CurveComponent::~CurveComponent()
{
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
    if (_IndexBuffer)
    {
        GetRenderable()->SetIndexBuffer(0);
        GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
        _IndexBuffer = 0;
    }
    
    if (_VertexBuffer)
    {
        GetRenderable()->SetVertexBuffer(0);
        GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
        _VertexBuffer = 0;
    }
    
    if (_Curve)
    {
        int size = _Curve->GetArcLength() / _MinDistance;
        
        _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, size*6);
        _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatStatic, pb::kVertexFormat_P3_C4_UV, size*4);
        
        GetRenderable()->SetIndexBuffer(_IndexBuffer);
        GetRenderable()->SetVertexBuffer(_VertexBuffer);
        
        _IndexBuffer->Lock();
        unsigned short* index = _IndexBuffer->GetData();
        for (int i=0; i<size; i++)
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

        float t = 0;
        glm::vec2 prevPoint = _Curve->EvaluateParam(t);
        
        _VertexBuffer->Lock();
        pb::Vertex_P3_C4_UV* vertices = static_cast<pb::Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
        for (int i=0; i<size; i++)
        {
            glm::vec2 point = _Curve->EvaluateParam(t);
            t += _MinDistance;
            
            float angle = glm::linearRand(0.f, 180.f);//0.f;
            float width = 5.f;
            
            for (int p=0; p<2; p++)
            {
                vertices[(i*4)+p].position[0] = prevPoint.x + cos(angle)*width + glm::linearRand(0.f,1.f);
                vertices[(i*4)+p].position[1] = prevPoint.y + sin(angle)*width + glm::linearRand(0.f,1.f);
                vertices[(i*4)+p].position[2] = 0;
                vertices[(i*4)+p].uv[0] = 0;
                vertices[(i*4)+p].uv[1] = t;
                vertices[(i*4)+p].color[0] = 1.f;
                vertices[(i*4)+p].color[1] = 1.f;
                vertices[(i*4)+p].color[2] = 1.f;
                vertices[(i*4)+p].color[3] = 1.f;
            }
            
            for (int p=2; p<4; p++)
            {
                vertices[(i*4)+p].position[0] = point.x + cos(angle)*width + glm::linearRand(0.f,1.f);
                vertices[(i*4)+p].position[1] = point.y + sin(angle)*width + glm::linearRand(0.f,1.f);
                vertices[(i*4)+p].position[2] = 0;
                vertices[(i*4)+p].uv[0] = 0;
                vertices[(i*4)+p].uv[1] = t;
                vertices[(i*4)+p].color[0] = 1.f;
                vertices[(i*4)+p].color[1] = 1.f;
                vertices[(i*4)+p].color[2] = 1.f;
                vertices[(i*4)+p].color[3] = 1.f;
            }
            
            for (int p=0; p<4; p++)
            {
                vertices[(i*4)+p].position[0] = glm::linearRand(-5.f,5.f);
                vertices[(i*4)+p].position[1] = glm::linearRand(-5.f,5.f);
                vertices[(i*4)+p].position[2] = 0;
            }
            
            prevPoint = point;
//            float i=0; i<_Curve->GetArcLength(); i+=_MinDistance
        }
        _VertexBuffer->Unlock();
        
        GetRenderable()->SetNumElements(size*6);
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

void OnUpdate(const Message& message)
{
    
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
