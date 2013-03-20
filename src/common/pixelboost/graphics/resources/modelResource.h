#include "pixelboost/asset/model.h"
#include "pixelboost/resource/resource.h"

namespace pb
{
    
    class File;
    
    class ModelResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        ModelResource(ResourcePool* pool, const std::string& filename);
        ~ModelResource();
        
    public:
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);
        
        const ModelDefinition& GetModelDefinition();
        
    private:
        bool LoadModel(File* file, ModelDefinition& model);
        bool LoadMesh(File* file, ModelMeshDefinition& mesh);
        bool LoadBone(File* file, ModelBoneDefinition& bone);
        bool LoadAnimation(File* file, ModelAnimationDefinition& animation);
        
        ModelDefinition _Model;
    };
    
}
