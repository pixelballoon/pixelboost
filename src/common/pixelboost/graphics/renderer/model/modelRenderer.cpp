#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <fstream>
#include <sstream>

#include <vector>

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/effect/manager.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"

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

Uid ModelRenderable::GetRenderableType()
{
    return TypeHash("model");
}

void ModelRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(_Transform);
}

Effect* ModelRenderable::GetEffect()
{
    Effect* baseEffect = Renderable::GetEffect();
    if (baseEffect)
        return baseEffect;
    
    return Renderer::Instance()->GetEffectManager()->GetEffect("/default/effects/textured.fx");
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
}

const glm::mat4x4& ModelRenderable::GetTransform()
{
    return _Transform;
}

Model::Model()
    : _RefCount(1)
{
    
}

Model::~Model()
{
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
}
    
bool Model::Load(const std::string& fileName)
{
    std::string objFilename = fileName;
    
    pb::File* file = pb::FileSystem::Instance()->OpenFile(pb::kFileLocationBundle, "");
                                                          
    if (!file)
        return false;
    
    std::string model;
    
    if (!file->ReadAll(model))
    {
        delete file;
        return false;
    }
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    std::vector<Vertex_NPXYZ_UV> verts;
    
    enum ReadMode
    {
        kReadModeVertex,
        kReadModeTexture,
        kReadModeNormal,
        kReadModeFace
    };
    
    ReadMode readMode;
    
    std::vector<std::string> lines;
    
    SplitString(model, '\n', lines);
    
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        std::string line = *it;
        
        std::vector<std::string> command = SplitLine(line);
        
        if (command.size() < 1)
            continue;
        
        if (command[0] == "v")
            readMode = kReadModeVertex;
        else if (command[0] == "vt")
            readMode = kReadModeTexture;
        else if (command[0] == "vn")
            readMode = kReadModeNormal;   
        else if (command[0] == "f")
            readMode = kReadModeFace;
        else
            continue;
        
        switch (readMode)
        {
            case kReadModeVertex:
            {
                vertices.push_back(glm::vec3(atof(command[1].c_str()), atof(command[2].c_str()), atof(command[3].c_str())));
                break;
            }
            case kReadModeTexture:
            {
                uvs.push_back(glm::vec2(atof(command[1].c_str()), atof(command[2].c_str())));
                break;
            }
            case kReadModeNormal:
            {
                normals.push_back(glm::vec3(atof(command[1].c_str()), atof(command[2].c_str()), atof(command[3].c_str())));
                break;
            }
            case kReadModeFace:
            {
                // Only support triangulated faces at the moment
                if (command.size() == 4)
                {
                    ParseVert(verts, command[1], vertices, uvs, normals);
                    ParseVert(verts, command[2], vertices, uvs, normals);
                    ParseVert(verts, command[3], vertices, uvs, normals);
                } else {
                    ParseVert(verts, command[1], vertices, uvs, normals);
                    ParseVert(verts, command[2], vertices, uvs, normals);
                    ParseVert(verts, command[3], vertices, uvs, normals);
                    ParseVert(verts, command[1], vertices, uvs, normals);
                    ParseVert(verts, command[3], vertices, uvs, normals);
                    ParseVert(verts, command[4], vertices, uvs, normals);
                }
                break;
            }
        }
    }

    delete file;
    
    _NumVertices = verts.size();

    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_NP_XYZ_UV, _NumVertices);
    _VertexBuffer->Lock();
    Vertex_NPXYZ_UV* vertexBuffer = static_cast<Vertex_NPXYZ_UV*>(_VertexBuffer->GetData());
    for (int i=0; i<_NumVertices; i++)
    {
        vertexBuffer[i].position[0] = verts[i].position[0];
        vertexBuffer[i].position[1] = verts[i].position[1];
        vertexBuffer[i].position[2] = verts[i].position[2];
        vertexBuffer[i].uv[0] = verts[i].uv[0];
        vertexBuffer[i].uv[1] = verts[i].uv[1];
        vertexBuffer[i].normal[0] = verts[i].normal[0];
        vertexBuffer[i].normal[1] = verts[i].normal[1];
        vertexBuffer[i].normal[2] = verts[i].normal[2];
    }
    _VertexBuffer->Unlock();
    
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, _NumVertices);
    _IndexBuffer->Lock();
    unsigned short* indexBuffer = _IndexBuffer->GetData();
    for (int i=0; i<_NumVertices; i++)
    {
        indexBuffer[i] = i;
    }
    _IndexBuffer->Unlock();
    
	return true;
}
    
void Model::ParseVert(std::vector<Vertex_NPXYZ_UV>& verts, const std::string& vert, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals)
{
    std::vector<std::string> vertIndices = SplitPath(vert);
    
    if (vertIndices.size() < 3)
        return;
    
    int posIndex = atoi(vertIndices[0].c_str());
    int uvIndex = atoi(vertIndices[1].c_str());
    int normalIndex = atoi(vertIndices[2].c_str());
    
    glm::vec3 pos = vertices[posIndex-1];
    glm::vec2 uv = uvs[uvIndex-1];
    glm::vec3 normal = normals[normalIndex-1];
    
    Vertex_NPXYZ_UV vertex;
    vertex.position[0] = pos[0];
    vertex.position[1] = pos[1];
    vertex.position[2] = pos[2];
    vertex.uv[0] = uv[0];
    vertex.uv[1] = 1.f-uv[1];
    vertex.normal[0] = normal[0];
    vertex.normal[1] = normal[1];
    vertex.normal[2] = normal[2];
    
    verts.push_back(vertex);
}
    
std::vector<std::string>& Model::SplitString(const std::string &string, char delim, std::vector<std::string> &items)
{
    std::stringstream stream(string);
    std::string item;
    while(std::getline(stream, item, delim)) {
        items.push_back(item);
    }
    return items;
}

std::vector<std::string> Model::SplitLine(const std::string &string)
{
    std::vector<std::string> items;
    return SplitString(string, ' ', items);
}


std::vector<std::string> Model::SplitPath(const std::string &string)
{
    std::vector<std::string> items;
    return SplitString(string, '/', items);
}

ModelRenderer::ModelRenderer()
{
    Renderer::Instance()->SetHandler(TypeHash("model"), this);
    
    Renderer::Instance()->GetEffectManager()->LoadEffect("/default/effects/textured.fx");
}

ModelRenderer::~ModelRenderer()
{
    Renderer::Instance()->GetEffectManager()->UnloadEffect("/default/effects/textured.fx");
    
    for (ModelMap::iterator it = _Models.begin(); it != _Models.end(); ++it)
    {
        delete it->second;
    }
    
    for (TextureMap::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        GraphicsDevice::Instance()->DestroyTexture(it->second);
    }
}

void ModelRenderer::Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
    for (int i=0; i<count; i++)
    {
        ModelRenderable& renderable = *static_cast<ModelRenderable*>(renderables[i]);
        
        Model* model = GetModel(renderable._Model);
        Texture* texture = GetTexture(renderable._Texture);
        
        if (!model || !texture)
            continue;
        
        effectPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", renderable.GetMVP());
        effectPass->GetShaderProgram()->SetUniform("diffuseColor", renderable._Tint);
        effectPass->GetShaderProgram()->SetUniform("diffuseTexture", 0);
        
        GraphicsDevice::Instance()->BindIndexBuffer(model->_IndexBuffer);
        GraphicsDevice::Instance()->BindVertexBuffer(model->_VertexBuffer);
        GraphicsDevice::Instance()->BindTexture(texture);
        
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, model->_NumVertices);
    }
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
  
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);

}
    
bool ModelRenderer::LoadModel(const std::string& modelName, const std::string& fileName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it != _Models.end())
    {
        it->second->_RefCount++;
        return true;
    }
    
    Model* model = new Model();
    model->Load(fileName);
    
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
    
bool ModelRenderer::LoadTexture(const std::string& textureName, const std::string& fileName, bool createMips)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it != _Textures.end())
    {
        return false;
    }
    
    Texture* texture = GraphicsDevice::Instance()->CreateTexture();
    texture->LoadFromPng(fileName, createMips);
    
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
