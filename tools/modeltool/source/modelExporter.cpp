#include "pixelboost/asset/model.h"
#include "pipeline/file/fileSystem.h"

#include "modelExporter.h"
#include "modelLoader.h"

ModelExporter::ModelExporter(ModelLoader* modelLoader)
    : _ModelLoader(modelLoader)
{
    
}

ModelExporter::~ModelExporter()
{
    delete _ModelLoader;
}

bool ModelExporter::Save(const std::string& filename) const
{
    if (!_ModelLoader)
        return false;
    
    const pb::ModelDefinition* model = _ModelLoader->GetModel();
    
    SaveModel(model, filename);
    
    return false;
}

bool ModelExporter::Process()
{
    if (!_ModelLoader)
        return false;
    
    if (!_ModelLoader->Process())
        return false;
    
    _ModelLoader->GetModel()->CalculateBounds();
    
    return true;    
}

bool ModelExporter::SaveModel(const pb::ModelDefinition* model, const std::string& filename) const
{
    pl::File* file = pl::FileSystem::Instance()->OpenFile(filename, pl::kFileModeWrite);
    
    if (!file)
        return false;
    
    file->Write(pb::ModelDefinition::kCurrentModelVersion);
    
    file->Write(static_cast<short>(model->Meshes.size()));
    
    for (std::vector<pb::ModelMeshDefinition>::const_iterator it = model->Meshes.begin(); it != model->Meshes.end(); ++it)
    {
        if (!SaveModelMesh(*it, file))
        {
            delete file;
            return false;
        }
    }
    
    file->Write(static_cast<short>(model->Bones.size()));
    
    for (std::vector<pb::ModelBoneDefinition>::const_iterator it = model->Bones.begin(); it != model->Bones.end(); ++it)
    {
        if (!SaveModelBone(*it, file))
        {
            delete file;
            return false;
        }
    }
    
    file->Write(static_cast<short>(model->Animations.size()));
    
    for (std::vector<pb::ModelAnimationDefinition>::const_iterator it = model->Animations.begin(); it != model->Animations.end(); ++it)
    {
        if (!SaveModelAnimation(*it, file))
        {
            delete file;
            return false;
        }
    }
    
    delete file;
    
    return true;
}

bool ModelExporter::SaveModelMesh(const pb::ModelMeshDefinition& mesh, pl::File* file) const
{
    file->Write(mesh.Indexed);
    file->Write(mesh.Skinned);
    file->Write(static_cast<short>(mesh.VertexFormat));
    file->Write(static_cast<short>(mesh.Vertices.size()));
    
    file->Write(mesh.Bounds.GetSize());
    
    for (std::vector<pb::ModelVertex>::const_iterator it = mesh.Vertices.begin(); it != mesh.Vertices.end(); ++it)
    {
        switch (mesh.VertexFormat)
        {
            case pb::kModelVertexFormat_1P_1N_1UV:
            {
                file->Write(it->Position.x);
                file->Write(it->Position.y);
                file->Write(it->Position.z);
                file->Write(it->Normal.x);
                file->Write(it->Normal.y);
                file->Write(it->Normal.z);
                file->Write(it->UV.x);
                file->Write(it->UV.y);
                break;
            }
            case pb::kModelVertexFormat_1P_1N_1UV_4BW:
            {
                file->Write(it->Position.x);
                file->Write(it->Position.y);
                file->Write(it->Position.z);
                file->Write(it->Normal.x);
                file->Write(it->Normal.y);
                file->Write(it->Normal.z);
                file->Write(it->UV.x);
                file->Write(it->UV.y);
                file->Write(it->Bone[0]);
                file->Write(it->Bone[1]);
                file->Write(it->Bone[2]);
                file->Write(it->Bone[3]);
                file->Write(it->BoneWeights[0]);
                file->Write(it->BoneWeights[1]);
                file->Write(it->BoneWeights[2]);
                file->Write(it->BoneWeights[3]);
                break;
            }
            default:
            {
                return false;
            }
        }
    }
    
    if (mesh.Indexed)
    {
        file->Write(static_cast<short>(mesh.Indices.size()));
        
        for (std::vector<short>::const_iterator it = mesh.Indices.begin(); it != mesh.Indices.end(); ++it)
        {
            file->Write(*it);
        }
    }
    
    return true;
}

bool ModelExporter::SaveModelBone(const pb::ModelBoneDefinition& bone, pl::File* file) const
{
    file->Write(static_cast<int>(bone._Name.length()));
    file->Write((unsigned char*)bone._Name.c_str(), bone._Name.length()+1);
    
    file->Write(bone._Id);
    file->Write(bone._ParentId);
    
    file->Write(bone._Position.x);
    file->Write(bone._Position.y);
    file->Write(bone._Position.z);
    
    file->Write(bone._Rotation.x);
    file->Write(bone._Rotation.y);
    file->Write(bone._Rotation.z);
    file->Write(bone._Rotation.w);
    
    for (int x=0; x<4; x++)
        for (int y=0; y<4; y++)
            file->Write(bone._BindMatrix[x][y]);
    
    return true;
}

bool ModelExporter::SaveModelAnimation(const pb::ModelAnimationDefinition& animation, pl::File* file) const
{
    file->Write(static_cast<int>(animation._Name.length()));
    file->Write((unsigned char*)animation._Name.c_str(), animation._Name.length()+1);
    
    file->Write(animation._FPS);
    file->Write(animation._Length);
    
    file->Write(static_cast<short>(animation._Frames.size()));
    for (std::vector<pb::ModelAnimationDefinition::AnimationFrame>::const_iterator frameIt = animation._Frames.begin(); frameIt != animation._Frames.end(); ++frameIt)
    {
        file->Write(static_cast<short>(frameIt->size()));
        for (std::vector<glm::mat4x4>::const_iterator matrixIt = frameIt->begin(); matrixIt != frameIt->end(); ++matrixIt)
        {
            for (int x=0; x<4; x++)
                for (int y=0; y<4; y++)
                    file->Write((*matrixIt)[x][y]);
        }
    }
    
    return true;
}
