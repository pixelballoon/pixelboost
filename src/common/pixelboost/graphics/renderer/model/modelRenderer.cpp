#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <fstream>
#include <sstream>

#include <vector>

#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/graphics/renderer/model/model.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"

using namespace pb;


ModelRenderable::ModelRenderable(Uid entityId)
    : Renderable(entityId)
{
    _AlphaBlend = false;
    _Model = 0;
    _Texture = 0;
    _Tint = glm::vec4(1,1,1,1);
}

ModelRenderable::~ModelRenderable()
{
}

Uid ModelRenderable::GetType()
{
    return ModelRenderable::GetStaticType();
}

Uid ModelRenderable::GetStaticType()
{
    return TypeHash("pb::ModelRenderable");
}

void ModelRenderable::CalculateBounds()
{
    if (!_Model)
        return;
    
    glm::vec4 position = GetWorldMatrix() * glm::vec4(0,0,0,1);
    float scale = glm::length(GetWorldMatrix() * glm::vec4(0.5774,0.5774,0.5774,0));
    
    BoundingSphere bounds = _Model->GetBounds();
    bounds.Set(glm::vec3(position.x, position.y, position.z), bounds.GetSize() * scale);
    SetBounds(bounds);
}

void ModelRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(_Transform);
}

Shader* ModelRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/data/shaders/pb_textured.shc");
}

void ModelRenderable::SetModel(Model* model)
{
    _Model = model;
}

Model* ModelRenderable::GetModel()
{
    return _Model;
}

void ModelRenderable::SetTexture(Texture* texture)
{
    _Texture = texture;
}

Texture* ModelRenderable::GetTexture()
{
    return _Texture;
}

void ModelRenderable::SetTint(const glm::vec4& tint)
{
    _Tint = tint;
}

const glm::vec4& ModelRenderable::GetTint() const
{
    return _Tint;
}

void ModelRenderable::SetAlphaBlend(bool alphaBlend)
{
    _AlphaBlend = alphaBlend;
}

bool ModelRenderable::GetAlphaBlend()
{
    return _AlphaBlend;
}

void ModelRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;
    DirtyWorldMatrix();
    DirtyBounds();
}

const glm::mat4x4& ModelRenderable::GetTransform() const
{
    return _Transform;
}

SkinnedAnimationState::SkinnedAnimationState(ModelDefinition* model)
    : _AnimationTime(0)
    , _Model(model)
{
    
}

SkinnedAnimationState::~SkinnedAnimationState()
{
    
}

void SkinnedAnimationState::SetAnimation(const std::string& animation)
{
    _Animation = 0;
    
    for (std::vector<ModelAnimationDefinition>::iterator it = _Model->Animations.begin(); it != _Model->Animations.end(); ++it)
    {
        if (it->_Name == animation)
            _Animation = &(*it);
    }
    
    if (_Animation == 0)
    {
        PbLogError("pb.graphics.model", "Can't find animation %s on model\n", animation.c_str());
    }
    
    _Matrices.clear();
    while (_Matrices.size() < _Model->Bones.size())
        _Matrices.push_back(glm::mat4x4());
    
    UpdateBoneMatrices(GetFrame());
}

void SkinnedAnimationState::AdvanceAnimation(float length)
{
    _AnimationTime += length;
    
    UpdateBoneMatrices(GetFrame());
}

int SkinnedAnimationState::GetFrame()
{
    if (!_Animation)
        return 0;
    
    return glm::mod(_AnimationTime, _Animation->_Length)*(float)_Animation->_FPS;
}

const glm::mat4x4& SkinnedAnimationState::GetBoneMatrix(int boneId)
{
    static const glm::mat4x4 identity;
    
    if (boneId == -1)
        return identity;
    
    return _Matrices[boneId];
}

void SkinnedAnimationState::SoftwareSkin(Model* model)
{   
    for (std::vector<ModelMesh*>::const_iterator it = model->GetMeshes().begin(); it != model->GetMeshes().end(); ++it)
    {
        ModelMesh* mesh = *it;
        ModelMeshDefinition* definition = mesh->_MeshDefinition;
        
        mesh->_VertexBuffer->Lock();
        
        Vertex_P3_N3_UV* vertices = static_cast<Vertex_P3_N3_UV*>(mesh->_VertexBuffer->GetData());
        
        for (int vertexIdx=0; vertexIdx<definition->Vertices.size(); vertexIdx++)
        {
            glm::vec4 vertexPosition = glm::vec4(definition->Vertices[vertexIdx].Position, 1);
            glm::vec4 position;
            
            for (int boneIdx=0; boneIdx<4; boneIdx++)
            {
                position += (GetBoneMatrix(definition->Vertices[vertexIdx].Bone[boneIdx]) * _Model->Bones[definition->Vertices[vertexIdx].Bone[boneIdx]]._BindMatrix * vertexPosition) * definition->Vertices[vertexIdx].BoneWeights[boneIdx];
            }
            
            vertices[vertexIdx].position[0] = position.x;
            vertices[vertexIdx].position[1] = position.y;
            vertices[vertexIdx].position[2] = position.z;
        }
        
        (*it)->_VertexBuffer->Unlock();
    }
}

void SkinnedAnimationState::UpdateBoneMatrices(int frame)
{
    for (std::vector<ModelBoneDefinition>::const_iterator it = _Model->Bones.begin(); it != _Model->Bones.end(); ++it)
    {
        UpdateBoneMatrix(*it, frame);
    }
}
    
void SkinnedAnimationState::UpdateBoneMatrix(const ModelBoneDefinition& bone, int frame)
{
    _Matrices[bone._Id] = GetBoneMatrix(bone._ParentId) * _Animation->_Frames[frame][bone._Id];
}

ModelMesh::ModelMesh(const std::string& fileName, ModelMeshDefinition* mesh)
    : _IndexBuffer(0)
    , _MeshDefinition(mesh)
    , _VertexBuffer(0)
{
    if (mesh->Indexed == true)
    {
        PbLogError("pb.graphics.mesh", "Only non index models are currently supported (%s)", fileName.c_str());
        return;
    }
    
    _Bounds = mesh->Bounds;
    _NumVertices = mesh->Vertices.size();
    
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P3_N3_UV, _NumVertices);
    _VertexBuffer->Lock();
    Vertex_P3_N3_UV* vertexBuffer = static_cast<Vertex_P3_N3_UV*>(_VertexBuffer->GetData());
    for (std::vector<ModelVertex>::iterator it = mesh->Vertices.begin(); it != mesh->Vertices.end(); ++it)
    {
        vertexBuffer->position[0] = it->Position.x;
        vertexBuffer->position[1] = it->Position.y;
        vertexBuffer->position[2] = it->Position.z;
        vertexBuffer->uv[0] = it->UV.x;
        vertexBuffer->uv[1] = it->UV.y;
        vertexBuffer->normal[0] = it->Normal.x;
        vertexBuffer->normal[1] = it->Normal.y;
        vertexBuffer->normal[2] = it->Normal.z;
        vertexBuffer++;
    }
    _VertexBuffer->Unlock();
    
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, _NumVertices);
    _IndexBuffer->Lock();
    unsigned short* indexBuffer = _IndexBuffer->GetData();
    for (int i=0; i<_NumVertices; i++)
    {
        *indexBuffer = i;
        indexBuffer++;
    }
    _IndexBuffer->Unlock();
}

ModelMesh::~ModelMesh()
{
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
}

unsigned short ModelMesh::GetNumVertices()
{
    return _NumVertices;
}

const BoundingSphere& ModelMesh::GetBounds()
{
    return _Bounds;
}

Model::Model()
    : _ModelDefinition(0)
    , _RefCount(1)
{
    
}

Model::~Model()
{
    for (std::vector<ModelMesh*>::iterator it = _Meshes.begin(); it != _Meshes.end(); ++it)
    {
        delete *it;
    }
    
    delete _ModelDefinition;
}
    
bool Model::Load(const std::string& fileName)
{
    std::string objFilename = fileName;
    
    _ModelDefinition = new ModelDefinition();
    if (!_ModelDefinition->Open(fileName))
        return false;
       
    if (_ModelDefinition->Meshes.size() == 0)
    {
        PbLogWarn("pb.assets", "Model (%s) is empty", fileName.c_str());
        return true;
    }
    
    for (std::vector<ModelMeshDefinition>::iterator it = _ModelDefinition->Meshes.begin(); it != _ModelDefinition->Meshes.end(); ++it)
    {
        _Meshes.push_back(new ModelMesh(fileName, &(*it)));
    }
    
    return true;
}

const BoundingSphere& Model::GetBounds()
{
    if (!_Bounds.IsValid())
    {
        for (std::vector<ModelMesh*>::iterator it = _Meshes.begin(); it != _Meshes.end(); ++it)
        {
            _Bounds.Expand((*it)->GetBounds());
        }
    }
    
    return _Bounds;
}

ModelDefinition* Model::GetDefinition()
{
    return _ModelDefinition;
}

const std::vector<ModelMesh*>& Model::GetMeshes()
{
    return _Meshes;
}

ModelRenderer::ModelRenderer()
{
    Renderer::Instance()->SetHandler(ModelRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/data/shaders/pb_textured.shc");
}

ModelRenderer::~ModelRenderer()
{
    Renderer::Instance()->GetShaderManager()->UnloadShader("/data/shaders/pb_textured.shc");
    
    for (ModelMap::iterator it = _Models.begin(); it != _Models.end(); ++it)
    {
        delete it->second;
    }
    
    for (TextureMap::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        GraphicsDevice::Instance()->DestroyTexture(it->second);
    }
}

void ModelRenderer::Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
    for (int i=0; i<count; i++)
    {
        ModelRenderable& renderable = *static_cast<ModelRenderable*>(renderables[i]);
        
        Model* model = renderable._Model;
        Texture* texture = renderable._Texture;
        
        if (!model || !texture || !model->GetMeshes().size())
            continue;
        
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, renderable._AlphaBlend);
        GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable.GetModelViewMatrix());
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseColor", renderable._Tint);
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
        
        for (std::vector<ModelMesh*>::const_iterator it = model->GetMeshes().begin(); it != model->GetMeshes().end(); ++it)
        {
            GraphicsDevice::Instance()->BindIndexBuffer((*it)->_IndexBuffer);
            GraphicsDevice::Instance()->BindVertexBuffer((*it)->_VertexBuffer);
            GraphicsDevice::Instance()->BindTexture(texture);
            
            GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, (*it)->GetNumVertices());
        }
    }
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
  
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);

}
    
Model* ModelRenderer::LoadModel(const std::string& modelName, const std::string& fileName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it != _Models.end())
    {
        it->second->_RefCount++;
        return it->second;
    }
    
    Model* model = new Model();
    if (!model->Load(fileName))
    {
        delete model;
        return 0;
    }
    
    _Models[modelName] = model;
    
    return model;
}

bool ModelRenderer::UnloadModel(const std::string& modelName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it == _Models.end())
        return false;
    
    it->second->_RefCount--;
    
    if (it->second->_RefCount == 0)
    {
        delete it->second;
        _Models.erase(it);
    }
    
    return true;
}
    
Texture* ModelRenderer::LoadTexture(const std::string& textureName, const std::string& fileName, bool createMips)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it != _Textures.end())
    {
        return it->second;
    }
    
    Texture* texture = GraphicsDevice::Instance()->CreateTexture();
    if (!texture->LoadFromFile(fileName, createMips))
    {
        GraphicsDevice::Instance()->DestroyTexture(texture);
        return 0;
    }
    
    _Textures[textureName] = texture;
    
    return texture;
}

bool ModelRenderer::UnloadTexture(const std::string& textureName)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it == _Textures.end())
        return false;
 
    GraphicsDevice::Instance()->DestroyTexture(it->second);
    _Textures.erase(it);
    
    return true;
}
    
Model* ModelRenderer::GetModel(const std::string& modelName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it == _Models.end())
        return 0;

    return it->second;
}
    
Texture* ModelRenderer::GetTexture(const std::string& textureName)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it == _Textures.end())
        return 0;
    
    return it->second;
}

#endif
