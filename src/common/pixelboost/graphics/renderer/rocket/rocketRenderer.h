#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "Rocket/Core.h"

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{
    class IndexBuffer;
    class RenderLayer;
    class Texture;
    class VertexBuffer;
    
    class RocketRenderable : public pb::Renderable
    {
    public:
        RocketRenderable(Rocket::Core::Context* context);
        virtual ~RocketRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Shader* GetShader();
        
    private:
        Rocket::Core::Context* _Context;
        
        friend class RocketRenderer;
    };
    
    class RocketRenderer : public Rocket::Core::RenderInterface, public pb::IRenderer
    {
    public:
        RocketRenderer();
        virtual ~RocketRenderer();
        
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass);
        
    public:
        virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);
        
        virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture);
        virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);
        virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);
        
        virtual void EnableScissorRegion(bool enable);
        virtual void SetScissorRegion(int x, int y, int width, int height);
        
        virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
        virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);
        virtual void ReleaseTexture(Rocket::Core::TextureHandle texture);
        
        virtual float GetHorizontalTexelOffset();
        virtual float GetVerticalTexelOffset();
        
        virtual void Release();

    private:
        void PurgeBuffer(bool force);
        
        std::map<VertexBuffer*, IndexBuffer*> _BufferMap;
        std::map<VertexBuffer*, Texture*> _TextureMap;
        
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        int _VertexCount;
        
        int _MaxQuads;
        Renderable* _Renderable;
        Viewport* _Viewport;
        ShaderPass* _ShaderPass;
        
        glm::vec4 _Color;
    };
}

#endif
