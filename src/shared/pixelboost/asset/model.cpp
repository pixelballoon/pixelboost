#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/asset/model.h"

using namespace pb;

const short ModelDefinition::kCurrentModelVersion = 3;

ModelVertex::ModelVertex()
{
    
}

ModelMeshDefinition::ModelMeshDefinition()
    : Indexed(false)
    , Skinned(false)
{
    
}

void ModelMeshDefinition::CalculateBounds()
{
    float maxBounds = 0;
    for (std::vector<ModelVertex>::const_iterator it = Vertices.begin(); it != Vertices.end(); ++it)
    {
        maxBounds = glm::max(maxBounds, glm::length(it->Position));
    }
    Bounds.Set(glm::vec3(0,0,0), maxBounds);
}

ModelBoneDefinition::ModelBoneDefinition()
{
    
}


ModelAnimationDefinition::ModelAnimationDefinition()
{
    
}

ModelDefinition::ModelDefinition()
{
    
}

void ModelDefinition::CalculateBounds()
{
    for (std::vector<ModelMeshDefinition>::iterator it = Meshes.begin(); it != Meshes.end(); ++it)
    {
        it->CalculateBounds();
    }
}
