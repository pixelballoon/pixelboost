#pragma once

#include "pixelboost/logic/component/graphics/renderable.h"
#include "pixelboost/graphics/renderer/buffer/bufferRenderer.h"

namespace pb
{
    
    class BufferRenderable;
    class Effect;
    class IndexBuffer;
    class Message;
    class Shader;
    class VertexBuffer;
    
    class BufferComponent : public RenderableComponent<BufferRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        BufferComponent(Entity* parent);
        virtual ~BufferComponent();
        
    public:
        void SetIndexBuffer(IndexBuffer* indexBuffer);
        IndexBuffer* GetIndexBuffer();
        
        void SetVertexBuffer(VertexBuffer* vertexBuffer);
        VertexBuffer* GetVertexBuffer();
        
        void SetNumElements(int numElements);
        int GetNumElements();
    };
    
}
