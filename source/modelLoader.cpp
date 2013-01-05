#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/renderer/model/model.h"

#include "modelLoader.h"

ModelLoader::~ModelLoader()
{
    
}

ObjLoader::ObjLoader(const std::string& filename)
    : _Filename(filename)
    , _Model(0)
{
    
}

ObjLoader::~ObjLoader()
{
    delete _Model;
}

bool ObjLoader::Process()
{
    if (_Model)
        return true;
    
    pb::File* file = pb::FileSystem::Instance()->OpenFile(pb::kFileLocationUser, _Filename);
    
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
    
    std::vector<pb::Vertex_P3_N3_UV> verts;
    
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
        
        std::vector<std::string> command;
        SplitString(line, ' ', command);
        
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
     
    _Model = new pb::ModelDefinition();
    
    pb::ModelObject object;
    
    object.Indexed = false;
    object.VertexFormat = pb::kVertexFormat_P3_N3_UV;
    
    for (int i=0; i<verts.size(); i++)
    {
        pb::ModelVertex vertex;
        vertex.Position = glm::vec3(verts[i].position[0], verts[i].position[1], verts[i].position[2]);
        vertex.Normal = glm::vec3(verts[i].normal[0], verts[i].normal[1], verts[i].normal[2]);
        vertex.UV = glm::vec2(verts[i].uv[0], verts[i].uv[1]);
        object.Vertices.push_back(vertex);
    }
    
    _Model->Objects.push_back(object);
    
    return true;
}

const pb::ModelDefinition* ObjLoader::GetModel() const
{
    return _Model;
}

pb::ModelDefinition* ObjLoader::GetModel()
{
    return _Model;
}

void ObjLoader::ParseVert(std::vector<pb::Vertex_P3_N3_UV>& verts, const std::string& vert, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals)
{
    std::vector<std::string> vertIndices;
    SplitString(vert, '/', vertIndices);
    
    if (vertIndices.size() < 2)
        return;
    
    pb::Vertex_P3_N3_UV vertex;
    
    int posIndex = atoi(vertIndices[0].c_str());
    glm::vec3 pos = vertices[posIndex-1];
    vertex.position[0] = pos[0];
    vertex.position[1] = pos[1];
    vertex.position[2] = pos[2];
    
    int uvIndex = atoi(vertIndices[1].c_str());
    glm::vec2 uv = uvs[uvIndex-1];
    vertex.uv[0] = uv[0];
    vertex.uv[1] = 1.f-uv[1];
    
    if (vertIndices.size() > 2)
    {
        int normalIndex = atoi(vertIndices[2].c_str());
        glm::vec3 normal = normals[normalIndex-1];
        vertex.normal[0] = normal[0];
        vertex.normal[1] = normal[1];
        vertex.normal[2] = normal[2];
    }
    
    verts.push_back(vertex);
}

std::vector<std::string>& ObjLoader::SplitString(const std::string &string, char delim, std::vector<std::string> &items)
{
    std::stringstream stream(string);
    std::string item;
    while(std::getline(stream, item, delim)) {
        items.push_back(item);
    }
    return items;
}

FbxLoader::FbxLoader(const std::string& filename)
    : _Filename(filename)
    , _Model(0)
{
    
}

FbxLoader::~FbxLoader()
{
    
}

void FbxLoader::ProcessMesh(FbxNode* node, FbxMesh* mesh, glm::mat4x4 globalTransform)
{
    if (!mesh->IsTriangleMesh())
    {
        FbxGeometryConverter geomConverter(_Manager);
        mesh = geomConverter.TriangulateMesh(mesh);
    }
    
    bool hasSkin = mesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
    
    if (mesh->GetDeformerCount() > 1)
    {
        printf("ERROR: Mesh has more than one deformer (%d)!\n", mesh->GetDeformerCount());
        return;
    }
    
    pb::ModelObject object;
    
    object.Indexed = false;
    object.VertexFormat = pb::kVertexFormat_P3_N3_UV;
    
    FbxStringList uvNames;
    mesh->GetUVSetNames(uvNames);
    
    int vertexCount = mesh->GetControlPointsCount();
    std::map<int, std::vector<std::string> > vertexBindings;
    std::map<int, std::vector<float> > vertexWeights;
    
    if (hasSkin)
    {
        FbxSkin* skinDeformer = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
        
        int clusterCount = skinDeformer->GetClusterCount();
        if (clusterCount)
        {
            if (skinDeformer->GetSkinningType() == FbxSkin::eRigid || skinDeformer->GetSkinningType() == FbxSkin::eLinear)
            {
                for (int clusterIndex=0; clusterIndex<clusterCount; ++clusterIndex)
                {
                    FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);
                    FbxNode* linkNode = cluster->GetLink();
                    if (!linkNode)
                        continue;
                    
                    if (cluster->GetLinkMode() != FbxCluster::eNormalize)
                    {
                        hasSkin = false;
                        printf("ERROR: Only normalised skin link clusters are currently supported, exporting without skinning!\n");
                        continue;
                    }
                    
                    FbxAMatrix matrix;
                    glm::mat4x4 bindMatrix;
                    cluster->GetTransformLinkMatrix(matrix);
                    for (int x=0; x<4; x++)
                        for (int y=0; y<4; y++)
                            bindMatrix[x][y] = matrix[x][y];
                    _Model->Bones[_BoneMap[cluster->GetLink()->GetName()]]._BindMatrix = bindMatrix;
                    
                    int vertexIndexCount = cluster->GetControlPointIndicesCount();
                    
                    for (int k = 0; k < vertexIndexCount; ++k)
                    {
                        int vertexIndex = cluster->GetControlPointIndices()[k];
                        
                        if (vertexIndex >= vertexCount)
                            continue;
                        
                        double weight = cluster->GetControlPointWeights()[k];
                        
                        if (weight == 0.0)
                            continue;
                        
                        vertexBindings[vertexIndex].push_back(cluster->GetLink()->GetName());
                        vertexWeights[vertexIndex].push_back(weight);
                    }
                }
                
                bool tooManyWeights = false;
                for (int vertexIndex=0; vertexIndex<vertexCount; vertexIndex++)
                {
                    if (vertexBindings[vertexIndex].size() > 4)
                    {
                        tooManyWeights = true;
                    }
                }
                
                if (tooManyWeights)
                {
                    hasSkin = false;
                    printf("ERROR: Vertex on mesh %s has too many weights (max 4), exporting without skinning!\n", node->GetName());
                }
            } else {
                hasSkin = false;
                printf("ERROR: Only rigid and linear skinning are currently supported, exporting without skinning!\n");
            }
        }
    }
    
    if (hasSkin)
    {
        object.VertexFormat = pb::kVertexFormat_P3_N3_UV_BW;
        object.Skinned = true;
    }
    
    for (int polyIdx=0; polyIdx<mesh->GetPolygonCount(); polyIdx++)
    {
        if (mesh->GetPolygonSize(polyIdx) != 3)
        {
            printf("ERROR: Unexpected poly size (%d)!\n", mesh->GetPolygonSize(polyIdx));
            continue;
        }
        
        for (int vertIdx=0; vertIdx<3; vertIdx++)
        {
            pb::ModelVertex vertex;
            FbxVector4 position;
            FbxVector4 normal;
            FbxVector2 uv;
            
            int vertexId = mesh->GetPolygonVertex(polyIdx, vertIdx);
            
            position = mesh->GetControlPoints()[vertexId];
            mesh->GetPolygonVertexNormal(polyIdx, vertIdx, normal);
            
            if (uvNames.GetCount())
                mesh->GetPolygonVertexUV(polyIdx, vertIdx, uvNames[0], uv);
            
            glm::vec4 transformedPosition;
            glm::vec4 transformedNormal;
            
            transformedPosition = globalTransform * glm::vec4(position[0], position[1], position[2], 1);
            transformedNormal = globalTransform * glm::vec4(normal[0], normal[1], normal[2], 1);
            
            vertex.Position = glm::vec3(transformedPosition.x, transformedPosition.y, transformedPosition.z);
            vertex.Normal = glm::vec3(transformedNormal.x, transformedNormal.y, transformedNormal.z);
            vertex.UV = glm::vec2(uv[0], 1.f-uv[1]);
            
            if (hasSkin)
            {
                for (int i=0; i<4; i++)
                {
                    vertex.Bone[i] = vertexBindings[vertexId].size() > i ? _BoneMap[vertexBindings[vertexId][i]] : 0;
                    vertex.BoneWeights[i] = vertexWeights[vertexId].size() > i ? vertexWeights[vertexId][i] : 0.f;
                }
            }
            
            object.Vertices.push_back(vertex);
        }
    }
    
    _Model->Objects.push_back(object);
}

void FbxLoader::ProcessSkeleton(FbxNode* node, FbxSkeleton* skeleton, glm::mat4x4 globalTransform, glm::vec3 position, glm::quat rotation)
{
    if (_BoneMap.find(node->GetName()) == _BoneMap.end())
    {
        int numBones = _BoneMap.size();
        _BoneMap[node->GetName()] = numBones;
        
        pb::ModelBone bone;
        
        bone._Name = node->GetName();
        
        bone._Id = numBones;
        
        if (_BoneMap.size() == 1)
            bone._ParentId = -1;
        else
            bone._ParentId = _BoneMap[node->GetParent()->GetName()];
        
        printf("%d : Add bone %s -> %d\n", numBones, node->GetName(), bone._ParentId);
        
        bone._Position = position;
        bone._Rotation = rotation;
        
        _Model->Bones.push_back(bone);
        
        int numAnimations = _Scene->GetSrcObjectCount(FBX_TYPE(FbxAnimStack));
        
        while (_Model->Animations.size() < numAnimations)
        {
            FbxAnimStack* animation = FbxCast<FbxAnimStack>(_Scene->GetSrcObject(FBX_TYPE(FbxAnimStack), 0));
            
            printf("Add animation %s\n", animation->GetName());
            
            pb::ModelAnimation modelAnimation;
            modelAnimation._FPS = 30;
            modelAnimation._Length = animation->GetLocalTimeSpan().GetDuration().GetSecondDouble();
            
            FbxTimeSpan timeSpan = animation->GetLocalTimeSpan();
            float start = timeSpan.GetStart().GetSecondDouble();
            float end = timeSpan.GetStop().GetSecondDouble();
            for (float t=start; t<end; t+=1.0/30.0)
            {
                pb::ModelAnimation::AnimationFrame frame;
                modelAnimation._Frames.push_back(frame);
            }
            _Model->Animations.push_back(modelAnimation);
        }

        FbxAnimEvaluator* animationEvaluator = _Scene->GetEvaluator();
        
        for (int animationIdx=0; animationIdx<numAnimations; animationIdx++)
        {
            FbxAnimStack* animation = FbxCast<FbxAnimStack>(_Scene->GetSrcObject(FBX_TYPE(FbxAnimStack), animationIdx));
            animationEvaluator->SetContext(animation);
            
            FbxTimeSpan timeSpan = animation->GetLocalTimeSpan();
            float start = timeSpan.GetStart().GetSecondDouble();
            float end = timeSpan.GetStop().GetSecondDouble();
            
            int frameIdx=0;
            for (float t=start; t<end; t+=1.0/30.0, frameIdx++)
            {
                FbxTime time;
                time.SetSecondDouble(t);
                
                FbxAMatrix fbxMatrix = animationEvaluator->GetNodeLocalTransform(node, time);
                glm::mat4x4 matrix;
                
                for (int x=0; x<4; x++)
                    for (int y=0; y<4; y++)
                        matrix[x][y] = fbxMatrix[x][y];
                
                _Model->Animations[animationIdx]._Frames[frameIdx].push_back(matrix);
            }
        }
    }
}

void FbxLoader::ParseAttribute(FbxNode* node, FbxNodeAttribute* attribute, glm::mat4x4 globalTransform, glm::vec3 position, glm::quat rotation, FbxNodeAttribute::EType processType)
{
    if(!attribute)
        return;
    
    FbxString attrName = attribute->GetName();

    if (attribute->GetAttributeType() != processType && processType != FbxNodeAttribute::eUnknown)
        return;
    
    if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
    {
        FbxMesh* mesh = static_cast<FbxMesh*>(attribute);
        
        ProcessMesh(node, mesh, globalTransform);
    }
    
    if (attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
    {
        FbxSkeleton* skeleton = static_cast<FbxSkeleton*>(attribute);
        
        ProcessSkeleton(node, skeleton, globalTransform, position, rotation);
    }
}

void FbxLoader::ParseNode(FbxNode* node, glm::mat4x4 parentTransform, FbxNodeAttribute::EType processType)
{
    fbxDouble3 translation = node->LclTranslation.Get();
    fbxDouble3 rotation = node->LclRotation.Get();
    fbxDouble3 scale = node->LclScaling.Get();
    
    FbxTransform::EInheritType transformInherit;
    node->GetTransformationInheritType(transformInherit); // TODO: Make sure we don't need to handle this for current models
    
    glm::mat4x4 localTransform;
       
    EFbxRotationOrder rotationOrder;
    node->GetRotationOrder(FbxNode::eDestinationPivot, rotationOrder);
    
    if (rotationOrder != eEulerXYZ)
    {
        printf("ERROR: Unsupported rotation order, currently only Euler XYZ is supported. Transforms may not look correct.\n");
    }
    
    glm::quat rotationQuaternion;
    rotationQuaternion = glm::rotate(rotationQuaternion, (float)rotation[2], glm::vec3(0,0,1));
    rotationQuaternion = glm::rotate(rotationQuaternion, (float)rotation[1], glm::vec3(0,1,0));
    rotationQuaternion = glm::rotate(rotationQuaternion, (float)rotation[0], glm::vec3(1,0,0));
    
    localTransform = glm::translate(localTransform, glm::vec3(translation[0], translation[1], translation[2]));
    localTransform = localTransform * glm::mat4_cast(rotationQuaternion);
    localTransform = glm::scale(localTransform, glm::vec3(scale[0], scale[1], scale[2]));
    
    glm::mat4x4 transform = parentTransform * localTransform;
    
    for(int i=0; i<node->GetNodeAttributeCount(); i++)
        ParseAttribute(node, node->GetNodeAttributeByIndex(i), transform, glm::vec3(translation[0], translation[1], translation[2]), rotationQuaternion, processType);
    
    for(int j=0; j<node->GetChildCount(); j++)
        ParseNode(node->GetChild(j), transform, processType);
}

bool FbxLoader::Process()
{
    if (_Model)
        return true;
    
    _Manager = FbxManager::Create();
    
    FbxIOSettings *ios = FbxIOSettings::Create(_Manager, IOSROOT);
    _Manager->SetIOSettings(ios);
    
    FbxImporter* importer = FbxImporter::Create(_Manager,"");
    
    if(!importer->Initialize(_Filename.c_str(), -1, _Manager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", importer->GetLastErrorString());
        return false;
    }
    
    _Scene = FbxScene::Create(_Manager, "myScene");
    importer->Import(_Scene);
    importer->Destroy();
    
    _Model = new pb::ModelDefinition();
    
    FbxNode* rootNode = _Scene->GetRootNode();
    
    rootNode->ResetPivotSetAndConvertAnimation(30.f);
    
    if(rootNode)
    {
        for(int i=0; i<rootNode->GetChildCount(); i++)
        {
            ParseNode(rootNode->GetChild(i), glm::mat4x4(), FbxNodeAttribute::eSkeleton);
        }
        for(int i=0; i<rootNode->GetChildCount(); i++)
        {
            ParseNode(rootNode->GetChild(i), glm::mat4x4(), FbxNodeAttribute::eMesh);
        }
    }
    
    _Manager->Destroy();
    
    return true;
}

const pb::ModelDefinition* FbxLoader::GetModel() const
{
    return _Model;
}

pb::ModelDefinition* FbxLoader::GetModel()
{
    return _Model;
}
