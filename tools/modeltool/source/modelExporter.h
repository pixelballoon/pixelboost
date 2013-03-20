#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

namespace pb
{
    class ModelDefinition;
    class ModelMeshDefinition;
    class ModelBoneDefinition;
    class ModelAnimationDefinition;
}

namespace pl
{
    class File;
}

class ModelLoader;

class ModelExporter
{
public:
    ModelExporter(ModelLoader* modelLoader);
    virtual ~ModelExporter();
    
    bool Save(const std::string& filename) const;
    bool Process();
    
private:
    bool SaveModel(const pb::ModelDefinition* model, const std::string& filename) const;
    bool SaveModelMesh(const pb::ModelMeshDefinition& mesh, pl::File* file) const;
    bool SaveModelBone(const pb::ModelBoneDefinition& bone, pl::File* file) const;
    bool SaveModelAnimation(const pb::ModelAnimationDefinition& animation, pl::File* file) const;
    
    ModelLoader* _ModelLoader;
    std::vector<glm::vec3> _Vertices;
};
