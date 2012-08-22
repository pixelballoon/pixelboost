#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

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
        BufferRenderable(Uid entityId, IndexBuffer* indexBuffer, VertexBuffer* vertexBuffer, Texture* texture, int numElements);
        virtual ~BufferRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Shader* GetShader();
        
        void SetBounds(BoundingSphere bounds);
        void SetLocalTransform(const glm::mat4x4& localTransform);
        
        void SetIndexBuffer(IndexBuffer* indexBuffer);
        IndexBuffer* GetIndexBuffer();
        
        void SetVertexBuffer(VertexBuffer* vertexBuffer);
        VertexBuffer* GetVertexBuffer();
        
        void SetTexture(Texture* texture);
        Texture* GetTexture();
        
        void SetNumElements(int numElements);
        int GetNumElements();

    private:
        BoundingSphere _Bounds;
        glm::mat4x4 _LocalTransform;
        
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        Texture* _Texture;
        int _NumElements;
        
        friend class BufferRenderer;
    };

    class BufferRenderer : public IRenderer
    {
    public:
        BufferRenderer();
        virtual ~BufferRenderer();
        
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass);
    };
    
}

#endif
