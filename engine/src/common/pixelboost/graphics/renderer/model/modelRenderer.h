#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/framework/definitions.h"
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
        ModelRenderable();
        ~ModelRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        void SetModel(Model* model);
        Model* GetModel();
        
        void SetTint(const glm::vec4& tint);
        const glm::vec4& GetTint() const;
        
        void SetTransform(const glm::mat4x4& transform);
        const glm::mat4x4& GetTransform() const;
        
    private:
        Model* _Model;
        glm::mat4x4 _Transform;
        glm::vec4 _Tint;
        
        friend class ModelRenderer;
    };
    
    class SkinnedAnimationState
    {
    public:
        SkinnedAnimationState(const ModelDefinition* model);
        ~SkinnedAnimationState();
        
    public:
        void SetAnimation(const std::string& name);
        
        void AdvanceAnimation(float length);
        
        int GetFrame();
        const glm::mat4x4& GetBoneMatrix(int boneId);
        
    public:
        void SoftwareSkin(Model* model);
        
    private:
        void UpdateBoneMatrices(int frame);
        void UpdateBoneMatrix(const ModelBoneDefinition& bone, int frame);
        
        const ModelDefinition* _Model;
        const ModelAnimationDefinition* _Animation;
        
        std::vector<glm::mat4x4> _Matrices;
        
        float _AnimationTime;
    };
    
    class ModelMesh
    {
    public:
        ModelMesh(const ModelMeshDefinition* mesh);
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
        
        const ModelMeshDefinition* _MeshDefinition;
        
        friend class ModelRenderer;
        friend class SkinnedAnimationState;
    };
    
    class Model
    {
    public:
        Model(const ModelDefinition* definition);
        virtual ~Model();
        
    public:
        const BoundingSphere& GetBounds();
        
        const ModelDefinition* GetDefinition();
        
        const std::vector<ModelMesh*>& GetMeshes();
        
    private:
        BoundingSphere _Bounds;
        
        std::vector<ModelMesh*> _Meshes;
        
        const ModelDefinition* _ModelDefinition;
        
        friend class ModelRenderer;
    };
    
    class ModelRenderer : public IRenderer
    {
    public:
        ModelRenderer();
        ~ModelRenderer();
        
        static ModelRenderer* Instance();
        
        void Render(RenderLayer* layer);
        
        Model* GetModel(const std::string& name, ModelDefinition* definition);
        
        void Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
        
    private:
        typedef std::map<std::string, Model*> ModelMap;
        typedef std::map<std::string, Texture*> TextureMap;
        
        ModelMap _Models;
        TextureMap _Textures;
        
        static ModelRenderer* _Instance;
    };
    
}
