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
    class ModelAnimationDefinition;
    class ModelBoneDefinition;
    class ModelDefinition;
    class ModelMeshDefinition;
    class ModelMesh;
    class Model;
    
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
        
        void SetModel(Model* model);
        Model* GetModel();
        
        void SetTexture(Texture* texture);
        Texture* GetTexture();
        
        void SetTint(const glm::vec4& tint);
        const glm::vec4& GetTint();
        
        void SetTransform(const glm::mat4x4& transform);
        const glm::mat4x4& GetTransform();
        
    private:
        Model* _Model;
        Texture* _Texture;
        glm::mat4x4 _Transform;
        glm::vec4 _Tint;
        
        friend class ModelRenderer;
    };
    
    class SkinnedAnimationState
    {
    public:
        SkinnedAnimationState(ModelDefinition* model);
        ~SkinnedAnimationState();
        
    public:
        void SetAnimation(const std::string& animation);
        
        void AdvanceAnimation(float length);
        
        int GetFrame();
        const glm::mat4x4& GetBoneMatrix(int boneId);
        
    public:
        void SoftwareSkin(Model* model);
        
    private:
        void UpdateBoneMatrices(int frame);
        void UpdateBoneMatrix(const ModelBoneDefinition& bone, int frame);
        
        ModelDefinition* _Model;
        ModelAnimationDefinition* _Animation;
        
        std::vector<glm::mat4x4> _Matrices;
        
        float _AnimationTime;
    };
    
    class ModelMesh
    {
    public:
        ModelMesh(const std::string& filename, ModelMeshDefinition* mesh);
        virtual ~ModelMesh();
        
    public:
        unsigned short GetNumVertices();
        
        const BoundingSphere& GetBounds();
        
    public:
        unsigned long _RefCount;
        
        unsigned short _NumVertices;
        
    private:
        BoundingSphere _Bounds;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        
        ModelMeshDefinition* _MeshDefinition;
        
        friend class ModelRenderer;
        friend class SkinnedAnimationState;
    };
    
    class Model
    {
    private:
        Model();
        virtual ~Model();
        
        bool Load(FileLocation location, const std::string& modelName);
        
    public:
        const BoundingSphere& GetBounds();
        
        ModelDefinition* GetDefinition();
        
        const std::vector<ModelMesh*>& GetMeshes();
        
    public:
        unsigned long _RefCount;
        
    private:
        BoundingSphere _Bounds;
        
        std::vector<ModelMesh*> _Meshes;
        
        ModelDefinition* _ModelDefinition;
        
        friend class ModelRenderer;
    };
    
    class ModelRenderer : public IRenderer
    {
    public:
        ModelRenderer();
        ~ModelRenderer();
        
        void Render(RenderLayer* layer);
        
        Model* LoadModel(FileLocation location, const std::string& modelName, const std::string& fileName);
        bool UnloadModel(const std::string& modelName);
        
        Texture* LoadTexture(FileLocation location, const std::string& textureName, const std::string& fileName, bool createMips=true, bool hasPremultipliedAlpha=false);
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
