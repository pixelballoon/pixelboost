#pragma once

#include <memory>
#include <set>
#include <string>

#include "pixelboost/logic/component/graphics/renderable.h"
#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"

namespace pb
{
    
    class BufferRenderable;
    class HermiteCurve2D;
    class IndexBuffer;
    class Message;
    class Shader;
    class SvgResource;
    class VertexBuffer;
    
    class CurveComponent : public RenderableComponent<BufferRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        CurveComponent(Entity* parent);
        virtual ~CurveComponent();
        
    public:
        void SetMinDistance(float minDistance);
        float GetMinDistance();
        
        void SetLengthScale(float lengthScale);
        float GetLengthScale();
        
        void SetWidth(float width);
        float GetWidth();
        
        void SetTint(const glm::vec4& tint);
        const glm::vec4& GetTint();
        
        void ResetCurves();
        void AddCurve(const HermiteCurve2D* curve, bool skipRefresh = false);
        void RemoveCurve(const HermiteCurve2D* curve, bool skipRefresh = false);
        const std::set<const HermiteCurve2D*>& GetCurves();

        void Refresh();

    private:
        void OnDebugRender(const Message& message);
        void OnSetColor(const Message& message);
        
        float _MinDistance;
        float _LengthScale;
        float _Width;
        glm::vec4 _Tint;

        std::set<const HermiteCurve2D*> _Curves;

        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
    };
    
    class CurveComponentSVG : public CurveComponent
    {
        PB_DECLARE_COMPONENT
        
    protected:
        CurveComponentSVG(Entity* parent);
        virtual ~CurveComponentSVG();
        
    public:
        void SetCurve(const std::string& resource, const std::string& curve);
        
    private:
        void OnUpdate(const Message& message);
        
        void OnResourceLoaded(Resource* resource, bool error);
        void OnResourceUnloading(Resource* resource);
        
        std::shared_ptr<pb::SvgResource> _Resource;
        std::string _CurveName;
    };
    
}
