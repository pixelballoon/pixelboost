#pragma once

#include <map>
#include <vector>

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{
    
    class IndexBuffer;
    class Texture;
    class VertexBuffer;

    class BufferRenderable : public Renderable
    {
    public:
        BufferRenderable();
        virtual ~BufferRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        void SetBounds(BoundingSphere bounds);
        void SetTransform(const glm::mat4x4& localTransform);
        
        void SetIndexBuffer(IndexBuffer* indexBuffer);
        IndexBuffer* GetIndexBuffer();
        
        void SetVertexBuffer(VertexBuffer* vertexBuffer);
        VertexBuffer* GetVertexBuffer();
        
        void SetNumElements(int numElements);
        int GetNumElements();

    private:
        BoundingSphere _Bounds;
        glm::mat4x4 _Transform;
        
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        int _NumElements;
        
        friend class BufferRenderer;
    };

    class BufferRenderer : public IRenderer
    {
    public:
        BufferRenderer();
        virtual ~BufferRenderer();
        
        static BufferRenderer* Instance();
        
        virtual void Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
        
    private:
        static BufferRenderer* _Instance;
    };
    
}
