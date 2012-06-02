#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"

namespace pb
{
    
class IndexBuffer;
    
class Model
{
public:
    Model();
    virtual ~Model();
    
    bool Load(const std::string& modelName);
    
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
    
    void Update(float time);
    void Render(RenderLayer* layer);
    
    bool LoadModel(const std::string& modelName);
    bool UnloadModel(const std::string& modelName);
    
    bool LoadTexture(const std::string& textureName, bool createMips=true);
    bool UnloadTexture(const std::string& textureName);
    
    bool AttachToRenderer(RenderLayer* layer, const std::string& modelName, const std::string& textureName, glm::vec3 position, glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f), glm::vec3 offset = glm::vec3(0.f, 0.f, 0.f));
    
private:
    struct ModelInstance
    {
        std::string modelName;
        std::string textureName;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 offset;
    };
    
    typedef std::vector<ModelInstance> InstanceList;
    typedef std::map<RenderLayer*, InstanceList> InstanceListMap;
    
    Model* GetModel(const std::string& modelName);
    Texture* GetTexture(const std::string& textureName);
    
    typedef std::map<std::string, Model*> ModelMap;
    typedef std::map<std::string, Texture*> TextureMap;
    
    ModelMap _Models;
    TextureMap _Textures;
    
    InstanceListMap _Instances;
};
    
}
