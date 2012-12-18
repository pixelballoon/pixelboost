#pragma once

#include <string>

#include "pixelboost/graphics/device/vertexBuffer.h"

namespace pb
{
    class ModelDefinition;
}

class ModelLoader
{
public:
    virtual ~ModelLoader();
    
    virtual bool Process() = 0;
    virtual const pb::ModelDefinition* GetModel() const = 0;
    virtual pb::ModelDefinition* GetModel() = 0;
};

class ObjLoader : public ModelLoader
{
public:
    ObjLoader(const std::string& filename);
    virtual ~ObjLoader();
    
    virtual bool Process();
    virtual const pb::ModelDefinition* GetModel() const;
    virtual pb::ModelDefinition* GetModel();
    
private:
    void ParseVert(std::vector<pb::Vertex_NPXYZ_UV>& verts, const std::string& vert, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals);
    std::vector<std::string>& SplitString(const std::string &string, char delim, std::vector<std::string> &items);
    
    std::string _Filename;
    pb::ModelDefinition* _Model;
};
