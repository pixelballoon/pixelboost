#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <fstream>
#include <sstream>

#include <vector>

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"

using namespace pb;

Model::Model()
    : _RefCount(1)
{
    
}

Model::~Model()
{
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
}
    
bool Model::Load(const std::string& modelName)
{
    std::string objFilename = FileHelpers::GetRootPath() + "/data/models/" + modelName + ".obj";
    
    FILE* file = fopen(objFilename.c_str(), "r");
    
    if (!file)
        return false;
    
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
    
    char lineChars[1024];
    while (!feof(file))
    {
        fgets(lineChars, 1024, file);
        
        std::string line = lineChars;
        
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
                }
                break;
            }
        }
    }

    fclose(file);
    
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
    
}

ModelRenderer::~ModelRenderer()
{
    for (ModelMap::iterator it = _Models.begin(); it != _Models.end(); ++it)
    {
        delete it->second;
    }
    
    for (TextureMap::iterator it = _Textures.begin(); it != _Textures.end(); ++it)
    {
        GraphicsDevice::Instance()->DestroyTexture(it->second);
    }
}

void ModelRenderer::Update(float time)
{
    _Instances.clear();
}

void ModelRenderer::Render(RenderLayer* layer)
{
    /*
    InstanceList& instanceList = _Instances[layer];
    if (!instanceList.size())
        return;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
    
    glAlphaFunc(GL_GREATER, 0.5f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    for (InstanceList::iterator it = instanceList.begin(); it != instanceList.end(); ++it)
    {
        Model* model = GetModel(it->modelName);
        Texture* texture = GetTexture(it->textureName);
        
        if (!model || !texture)
            continue;
        
        glPushMatrix();
        
        glTranslatef(it->position[0], it->position[1], -it->position[2]);
        glScalef(it->scale[0], it->scale[1], it->scale[2]);
        glTranslatef(it->offset[0], it->offset[1], it->offset[2]);
        glRotatef(it->rotation[0], 1, 0, 0);
        glRotatef(it->rotation[1], 0, 1, 0);
        glRotatef(it->rotation[2], 0, 0, 1);
        
        GraphicsDevice::Instance()->BindIndexBuffer(model->_IndexBuffer);
        GraphicsDevice::Instance()->BindVertexBuffer(model->_VertexBuffer);
        GraphicsDevice::Instance()->BindTexture(texture);
        
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, model->_NumVertices);
        
        glPopMatrix();
    }
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    */
}
    
bool ModelRenderer::LoadModel(const std::string& modelName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it != _Models.end())
    {
        it->second->_RefCount++;
        return true;
    }
    
    Model* model = new Model();
    model->Load(modelName);
    
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
    
bool ModelRenderer::LoadTexture(const std::string& textureName, bool createMips)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it != _Textures.end())
    {
        return false;
    }
    
    Texture* texture = GraphicsDevice::Instance()->CreateTexture();
    texture->LoadFromPng(FileHelpers::GetRootPath() + "/data/models/" + textureName + ".png", createMips);
    
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

bool ModelRenderer::AttachToRenderer(RenderLayer* layer, const std::string& modelName, const std::string& textureName, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 offset)
{
    Model* model = GetModel(modelName);
    Texture* texture = GetTexture(textureName);
    
    if (!model || !texture)
        return false;
    
    ModelInstance instance;
    instance.modelName = modelName;
    instance.textureName = textureName;
    instance.position = position;
    instance.rotation = rotation;
    instance.scale = scale;
    instance.offset = offset;

    _Instances[layer].push_back(instance);
    
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
