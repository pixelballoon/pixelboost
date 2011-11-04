#pragma once

#include <map>
#include <string>

#include "libpixel/graphics/device/texture.h"
#include "libpixel/graphics/device/vertexBuffer.h"
#include "libpixel/graphics/render/common/irenderer.h"
#include "libpixel/math/maths.h"

namespace libpixel
{
    
class Model
{
public:
    Model();
    virtual ~Model();
    
    bool Load(const std::string& modelName);
    
public:
    unsigned long _RefCount;
    
    GLuint _NumVertices;
    GLuint _VertexBuffer;
    GLuint _IndexBuffer;
    
private:
    void ParseVert(std::vector<Vertex_NPXYZ_UV>& verts, const std::string& vert, const std::vector<Vec3>& vertices, const std::vector<Vec2>& uvs, const std::vector<Vec3>& normals);
    
    std::vector<std::string>& SplitString(const std::string& string, char delim, std::vector<std::string> &items);
    std::vector<std::string> SplitLine(const std::string& string);
    std::vector<std::string> SplitPath(const std::string& string);
};
    
class ModelTexture
{
public:
    ModelTexture();
    virtual ~ModelTexture();
    
    bool Load(const std::string& textureName);
    
public:
    unsigned long _RefCount;
    
    Texture* _Texture;
};

class ModelRenderer
{
public:
    ModelRenderer();
    ~ModelRenderer();
    
    bool LoadModel(const std::string& modelName);
    bool UnloadModel(const std::string& modelName);
    
    bool LoadTexture(const std::string& textureName);
    bool UnloadTexture(const std::string& textureName);
    
    bool Render(const std::string& modelName, const std::string& textureName, Vec3 position, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec3 scale = Vec3(1.f, 1.f, 1.f), Vec3 offset = Vec3(0.f, 0.f, 0.f));
    
private:
    Model* GetModel(const std::string& modelName);
    ModelTexture* GetTexture(const std::string& textureName);
    
    typedef std::map<std::string, Model*> ModelMap;
    typedef std::map<std::string, ModelTexture*> TextureMap;
    
    ModelMap _Models;
    TextureMap _Textures;
};
    
}
