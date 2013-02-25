#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{
    
    class BufferRenderable;
    class Effect;
    class IndexBuffer;
    class Message;
    class Shader;
    class VertexBuffer;
    
    class BufferComponent : public Component
    {
    public:
        BufferComponent(Entity* entity, IndexBuffer* indexBuffer = 0, VertexBuffer* vertexBuffer = 0, Texture* texture = 0, int numElements = 0);
        virtual ~BufferComponent();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        void SetLayer(int layer);
        
        void SetIndexBuffer(IndexBuffer* indexBuffer);
        IndexBuffer* GetIndexBuffer();
        
        void SetVertexBuffer(VertexBuffer* vertexBuffer);
        VertexBuffer* GetVertexBuffer();
        
        void SetTexture(Texture* texture);
        Texture* GetTexture();
        
        void SetShader(Shader* shader);
        Shader* GetShader();
        
        void SetNumElements(int numElements);
        int GetNumElements();
        
        void SetLocalTransform(const glm::mat4x4& transform);
        
    private:
        void OnTransformChanged(const Message& message);
        void UpdateTransform();
        
        glm::mat4x4 _LocalTransform;

        BufferRenderable* _Renderable;
    };
    
}