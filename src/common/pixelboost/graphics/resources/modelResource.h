#include "pixelboost/asset/model.h"
#include "pixelboost/resource/resource.h"

namespace pb
{
    
    class ModelResource : public pb::Resource
    {
    public:
        ModelResource();
        ~ModelResource();
        
        virtual Uid GetResourceType();
        static Uid GetStaticResourceType();
        
        virtual bool ProcessResource(ResourceState state, const std::string& filename, std::string& error);
        static ResourceThread GetResourceThread(ResourceState state);
        
        const ModelDefinition& GetModelDefinition();
        
    private:
        bool LoadModel(File* file, ModelDefinition& model);
        bool LoadMesh(File* file, ModelMeshDefinition& mesh);
        bool LoadBone(File* file, ModelBoneDefinition& bone);
        bool LoadAnimation(File* file, ModelAnimationDefinition& animation);
        
        ModelDefinition _Model;
    };
    
}
