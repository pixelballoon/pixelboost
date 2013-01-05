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
    _Model = "";
    _Texture = "";
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
    Model* model = Engine::Instance()->GetModelRenderer()->GetModel(_Model);
    
    if (!model)
        return;
    
    glm::vec4 position = GetWorldMatrix() * glm::vec4(0,0,0,1);
    float scale = glm::length(GetWorldMatrix() * glm::vec4(0.5774,0.5774,0.5774,0));
    
    BoundingSphere bounds = model->GetBounds();
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

void ModelRenderable::SetModel(const std::string& model)
{
    _Model = model;
}

const std::string& ModelRenderable::GetModel()
{
    return _Model;
}

void ModelRenderable::SetTexture(const std::string& texture)
{
    _Texture = texture;
}

const std::string& ModelRenderable::GetTexture()
{
    return _Texture;
}

void ModelRenderable::SetTint(const glm::vec4& tint)
{
    _Tint = tint;
}

const glm::vec4& ModelRenderable::GetTint()
{
    return _Tint;
}

void ModelRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;
    DirtyWorldMatrix();
    DirtyBounds();
}

const glm::mat4x4& ModelRenderable::GetTransform()
{
    return _Transform;
}

ModelMesh::ModelMesh(const std::string& fileName, ModelObject* object)
    : _IndexBuffer(0)
    , _ModelObject(object)
    , _VertexBuffer(0)
{
    if (object->Indexed == true)
    {
        PbLogError("pb.assets", "Only non index models are currently supported (%s)", fileName.c_str());
        return;
    }
    
    _Bounds = object->Bounds;
    _NumVertices = object->Vertices.size();
    
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P3_N3_UV, _NumVertices);
    _VertexBuffer->Lock();
    Vertex_P3_N3_UV* vertexBuffer = static_cast<Vertex_P3_N3_UV*>(_VertexBuffer->GetData());
    for (std::vector<ModelVertex>::iterator it = object->Vertices.begin(); it != object->Vertices.end(); ++it)
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
    
bool Model::Load(FileLocation location, const std::string& fileName)
{
    std::string objFilename = fileName;
    
    _ModelDefinition = new ModelDefinition();
    if (!_ModelDefinition->Open(location, fileName))
        return false;
       
    if (_ModelDefinition->Objects.size() == 0)
    {
        PbLogWarn("pb.assets", "Model (%s) is empty", fileName.c_str());
        return true;
    }
    
    for (std::vector<ModelObject>::iterator it = _ModelDefinition->Objects.begin(); it != _ModelDefinition->Objects.end(); ++it)
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
        
        Model* model = GetModel(renderable._Model);
        Texture* texture = GetTexture(renderable._Texture);
        
        if (!model || !texture || !model->GetMeshes().size())
            continue;
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewProj", renderable.GetMVP());
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
    
bool ModelRenderer::LoadModel(FileLocation location, const std::string& modelName, const std::string& fileName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it != _Models.end())
    {
        it->second->_RefCount++;
        return true;
    }
    
    Model* model = new Model();
    model->Load(location, fileName);
    
    _Models[modelName] = model;
    
    return false;
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
    
bool ModelRenderer::LoadTexture(FileLocation location, const std::string& textureName, const std::string& fileName, bool createMips, bool hasPremultipliedAlpha)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it != _Textures.end())
    {
        return false;
    }
    
    Texture* texture = GraphicsDevice::Instance()->CreateTexture();
    texture->LoadFromFile(location, fileName, createMips, hasPremultipliedAlpha);
    
    _Textures[textureName] = texture;
    
    return true;
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
