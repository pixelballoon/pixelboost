#pragma once

#include <map>
#include <string>

#include "fbxsdk.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"

#include "pixelboost/asset/model.h"

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
    void ParseVert(std::vector<pb::ModelVertex>& verts, const std::string& vert, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals);
    std::vector<std::string>& SplitString(const std::string &string, char delim, std::vector<std::string> &items);
    
    std::string _Filename;
    pb::ModelDefinition* _Model;
};

class FbxLoader : public ModelLoader
{
public:
    FbxLoader(const std::string& filename);
    virtual ~FbxLoader();
    
    virtual bool Process();
    virtual const pb::ModelDefinition* GetModel() const;
    virtual pb::ModelDefinition* GetModel();
    
private:
    void ParseNode(FbxNode* node, glm::mat4x4 transform, FbxNodeAttribute::EType processType = FbxNodeAttribute::eUnknown);
    void ParseAttribute(FbxNode* node, FbxNodeAttribute* attribute, glm::mat4x4 transform, glm::vec3 position, glm::quat rotation, FbxNodeAttribute::EType processType);
    
    void ProcessMesh(FbxNode* node, FbxMesh* mesh, glm::mat4x4 transform);
    void ProcessSkeleton(FbxNode* node, FbxSkeleton* skeleton, glm::mat4x4 transform, glm::vec3 position, glm::quat rotation);
    
    std::string _Filename;
    pb::ModelDefinition* _Model;
    
    std::map<std::string, int> _BoneMap;
    
    FbxManager* _Manager;
    FbxScene* _Scene;
};
