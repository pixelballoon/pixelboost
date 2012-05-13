#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/math/maths.h"

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
    void ParseVert(std::vector<Vertex_NPXYZ_UV>& verts, const std::string& vert, const std::vector<Vec3>& vertices, const std::vector<Vec2>& uvs, const std::vector<Vec3>& normals);
    
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
    
    bool AttachToRenderer(RenderLayer* layer, const std::string& modelName, const std::string& textureName, Vec3 position, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec3 scale = Vec3(1.f, 1.f, 1.f), Vec3 offset = Vec3(0.f, 0.f, 0.f));
    
private:
    struct ModelInstance
    {
        std::string modelName;
        std::string textureName;
        Vec3 position;
        Vec3 rotation;
        Vec3 scale;
        Vec3 offset;
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
