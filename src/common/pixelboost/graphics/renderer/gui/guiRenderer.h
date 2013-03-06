#pragma once

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{
    class IndexBuffer;
    class RenderLayer;
    class VertexBuffer;
    struct Vertex_P3_UV;
    
    class GuiRenderable : public pb::Renderable
    {
    public:
        GuiRenderable(Uid entityUid);
        virtual ~GuiRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Shader* GetShader();
        
        void SetTransform(const glm::mat4x4& transform);
        
    private:
        glm::mat4x4 _Transform;

    private:
        friend class GuiRenderer;
    };
    
    class GuiRenderer : public pb::IRenderer
    {
    public:
        GuiRenderer();
        virtual ~GuiRenderer();
        
        static GuiRenderer* Instance();
        
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass);
        
    private:
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;

        int _MaxQuads;
        
        static GuiRenderer* _Instance;
    };
}
