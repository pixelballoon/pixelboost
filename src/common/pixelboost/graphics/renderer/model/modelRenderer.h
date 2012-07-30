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

namespace pb
{
    
    class IndexBuffer;
    
    class ModelRenderable : public Renderable
    {
    public:
        ModelRenderable(Uid entityId);
        ~ModelRenderable();
        
        virtual Uid GetRenderableType();
        
        virtual void CalculateWorldMatrix();
        
        virtual Effect* GetEffect();
        
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
    public:
        Model();
        virtual ~Model();
        
        bool Load(FileLocation location, const std::string& modelName);
        
    public:
        unsigned long _RefCount;
        
        unsigned int _NumVertices;
        
    private:
        void ParseVert(std::vector<Vertex_NPXYZ_UV>& verts, const std::string& vert, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals);
        
        std::vector<std::string>& SplitString(const std::string& string, char delim, std::vector<std::string> &items);
        std::vector<std::string> SplitLine(const std::string& string);
        std::vector<std::string> SplitPath(const std::string& string);
        
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        
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
        
        void Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass);
        
    private:
        Model* GetModel(const std::string& modelName);
        Texture* GetTexture(const std::string& textureName);
        
        typedef std::map<std::string, Model*> ModelMap;
        typedef std::map<std::string, Texture*> TextureMap;
        
        ModelMap _Models;
        TextureMap _Textures;
    };
    
}
