#include <fstream>
#include <sstream>

#include <vector>

#include "pixelboost/asset/model.h"
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
#include "pixelboost/graphics/material/material.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/resource/resourceManager.h"

using namespace pb;

ModelRenderer* ModelRenderer::_Instance = 0;

ModelRenderable::ModelRenderable()
{
    _Model = 0;
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

void ModelRenderable::SetModel(Model* model)
{
    _Model = model;
}

Model* ModelRenderable::GetModel()
{
    return _Model;
}

void ModelRenderable::SetTint(const glm::vec4& tint)
{
    _Tint = tint;
}

const glm::vec4& ModelRenderable::GetTint() const
{
    return _Tint;
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

SkinnedAnimationState::SkinnedAnimationState(const ModelDefinition* model)
    : _AnimationTime(0)
    , _Model(model)
{
    
}

SkinnedAnimationState::~SkinnedAnimationState()
{
    
}

void SkinnedAnimationState::SetAnimation(const std::string& name)
{
    _Animation = 0;
    
    for (const auto& animation : _Model->Animations)
    {
        if (animation._Name == name)
            _Animation = &animation;
    }
    
    if (_Animation == 0)
    {
        PbLogError("pb.graphics.model", "Can't find animation %s on model\n", name.c_str());
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
        const ModelMeshDefinition* definition = mesh->_MeshDefinition;
        
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

ModelMesh::ModelMesh(const ModelMeshDefinition* mesh)
    : _IndexBuffer(0)
    , _MeshDefinition(mesh)
    , _VertexBuffer(0)
{
    if (mesh->Indexed == true)
    {
        PbLogError("pb.graphics.mesh", "Only non index models are currently supported");
        return;
    }
    
    _Bounds = mesh->Bounds;
    _NumVertices = mesh->Vertices.size();
    
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P3_N3_UV, _NumVertices);
    _VertexBuffer->Lock();
    Vertex_P3_N3_UV* vertexBuffer = static_cast<Vertex_P3_N3_UV*>(_VertexBuffer->GetData());
    for (const auto& vertex : mesh->Vertices)
    {
        vertexBuffer->position[0] = vertex.Position.x;
        vertexBuffer->position[1] = vertex.Position.y;
        vertexBuffer->position[2] = vertex.Position.z;
        vertexBuffer->uv[0] = vertex.UV.x;
        vertexBuffer->uv[1] = vertex.UV.y;
        vertexBuffer->normal[0] = vertex.Normal.x;
        vertexBuffer->normal[1] = vertex.Normal.y;
        vertexBuffer->normal[2] = vertex.Normal.z;
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

Model::Model(const ModelDefinition* definition)
    : _ModelDefinition(definition)
{
    if (_ModelDefinition->Meshes.size() == 0)
    {
        PbLogWarn("pb.graphics.model", "Model is empty");
        return;
    }
    
    for (const auto& mesh : _ModelDefinition->Meshes)
    {
        _Meshes.push_back(new ModelMesh(&mesh));
    }
}

Model::~Model()
{
    for (std::vector<ModelMesh*>::iterator it = _Meshes.begin(); it != _Meshes.end(); ++it)
    {
        delete *it;
    }
    
    delete _ModelDefinition;
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

const ModelDefinition* Model::GetDefinition()
{
    return _ModelDefinition;
}

const std::vector<ModelMesh*>& Model::GetMeshes()
{
    return _Meshes;
}

ModelRenderer::ModelRenderer()
{
    PbAssert(!_Instance);
    
    _Instance = this;
    
    Renderer::Instance()->SetHandler(ModelRenderable::GetStaticType(), this);
}

ModelRenderer::~ModelRenderer()
{
    _Instance = 0;
    
    for (ModelMap::iterator it = _Models.begin(); it != _Models.end(); ++it)
    {
        delete it->second;
    }
    
    for (TextureMap::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        GraphicsDevice::Instance()->DestroyTexture(it->second);
    }
}

ModelRenderer* ModelRenderer::Instance()
{
    return _Instance;
}

void ModelRenderer::Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    Material* material = renderables[0]->GetMaterial();
    
    if (!material)
        return;
    
    ShaderPass* shaderPass = material->Bind(renderScheme, 0, projectionMatrix, viewMatrix);
    
    if (!shaderPass)
        return;

    for (int i=0; i<count; i++)
    {
        ModelRenderable& renderable = *static_cast<ModelRenderable*>(renderables[i]);
        
        Model* model = renderable._Model;
        
        if (!model || !model->GetMeshes().size())
            continue;
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable.GetModelViewMatrix());
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseColor", renderable._Tint);
        
        for (std::vector<ModelMesh*>::const_iterator it = model->GetMeshes().begin(); it != model->GetMeshes().end(); ++it)
        {
            GraphicsDevice::Instance()->BindIndexBuffer((*it)->_IndexBuffer);
            GraphicsDevice::Instance()->BindVertexBuffer((*it)->_VertexBuffer);
            
            GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, (*it)->GetNumVertices());
        }
    }
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(0, 0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
  
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
}
