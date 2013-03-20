#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{
    
    class IndexBuffer;
    class ShaderPass;
    class VertexBuffer;
    class Viewport;
        
    class PrimitiveRenderable : public Renderable
    {
    public:
        PrimitiveRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        enum Type
        {
            kTypeEllipse,
            kTypeLine,
            kTypeRectangle,
        };
        
        virtual Shader* GetShader();
        virtual Type GetPrimitiveType() = 0;
        
        void SetTransform(const glm::mat4x4& transform);
        
        glm::vec4 GetColor();
        void SetColor(glm::vec4 color);
        
    private:
        glm::vec4 _Color;
        
    protected:
        glm::mat4x4 _Transform;
        
        friend class PrimitiveRenderer;
    };
        
    class PrimitiveRenderableEllipse : public PrimitiveRenderable
    {
    public:
        PrimitiveRenderableEllipse();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Type GetPrimitiveType();
        
        bool GetSolid();
        void SetSolid(bool solid);
        
        glm::vec3 GetPosition();
        void SetPosition(glm::vec3 position);
        
        glm::vec3 GetRotation();
        void SetRotation(glm::vec3 rotation);
        
        glm::vec2 GetSize();
        void SetSize(glm::vec2 size);
        
    private:
        bool _Solid;
        glm::vec3 _Position;
        glm::vec3 _Rotation;
        glm::vec2 _Size;
        
        friend class PrimitiveRenderer;
    };

    class PrimitiveRenderableLine : public PrimitiveRenderable
    {
    public:
        PrimitiveRenderableLine();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Type GetPrimitiveType();
        
        glm::vec3 GetStart();
        void SetStart(glm::vec3 start);
        
        glm::vec3 GetEnd();
        void SetEnd(glm::vec3 end);
        
    private:
        glm::vec3 _Start;
        glm::vec3 _End;
        
        friend class PrimitiveRenderer;
    };

        
    class PrimitiveRenderableRectangle : public PrimitiveRenderable
    {
    public:
        PrimitiveRenderableRectangle();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Type GetPrimitiveType();
        
        bool GetSolid();
        void SetSolid(bool solid);
        
        glm::vec2 GetSize();
        void SetSize(glm::vec2 size);
        
    private:
        bool _Solid;
        
        glm::vec2 _Size;
        
        friend class PrimitiveRenderer;
    };
        
    class PrimitiveRenderer : public IRenderer
    {
    public:
        PrimitiveRenderer();
        ~PrimitiveRenderer();
        
        static PrimitiveRenderer* Instance();
        
        virtual void Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
        
    private:
        IndexBuffer* _EllipseIndexBuffer;
        VertexBuffer* _EllipseVertexBuffer;
        
        IndexBuffer* _LineIndexBuffer;
        VertexBuffer* _LineVertexBuffer;
        
        IndexBuffer* _BoxIndexBuffer;
        VertexBuffer* _BoxVertexBuffer;
        
        static PrimitiveRenderer* _Instance;
    };
    
}
