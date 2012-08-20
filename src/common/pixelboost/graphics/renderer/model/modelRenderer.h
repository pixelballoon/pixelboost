#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/db/definitions.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/maths/boundingSphere.h"

namespace pb
{
    
    class IndexBuffer;
    class ModelDefinition;
    
    class ModelRenderable : public Renderable
    {
    public:
        ModelRenderable(Uid entityId);
        ~ModelRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Shader* GetShader();
        
        void SetModel(const std::string& model);
        const std::string& GetModel();
        
        void SetTexture(const std::string& texture);
        const std::string& GetTexture();
        
        void SetTint(const glm::vec4& tint);
        const glm::vec4& GetTint();
        
        void SetTransform(const glm::mat4x4& transform);
        const glm::mat4x4& GetTransform();
        
    private:
        std::string _Model;
        std::string _Texture;
        glm::mat4x4 _Transform;
        glm::vec4 _Tint;
        
        friend class ModelRenderer;
    };
    
    class Model
    {
    private:
        Model();
        virtual ~Model();
        
    public:
        bool Load(FileLocation location, const std::string& modelName);
        
        unsigned short GetNumVertices();
        
        const BoundingSphere& GetBounds();
        
    public:
        unsigned long _RefCount;
        
        unsigned short _NumVertices;
        
    private:
        BoundingSphere _Bounds;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        
        ModelDefinition* _ModelDefinition;
        
        friend class ModelRenderer;
    };

    class ModelRenderer : public IRenderer
    {
    public:
        ModelRenderer();
        ~ModelRenderer();
        
        void Render(RenderLayer* layer);
        
        bool LoadModel(FileLocation location, const std::string& modelName, const std::string& fileName);
        bool UnloadModel(const std::string& modelName);
        
        bool LoadTexture(FileLocation location, const std::string& textureName, const std::string& fileName, bool createMips=true);
        bool UnloadTexture(const std::string& textureName);
        
        void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass);
        
        Model* GetModel(const std::string& modelName);
        Texture* GetTexture(const std::string& textureName);
        
    private:
        typedef std::map<std::string, Model*> ModelMap;
        typedef std::map<std::string, Texture*> TextureMap;
        
        ModelMap _Models;
        TextureMap _Textures;
    };
    
}
