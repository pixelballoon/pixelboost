#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/model/model.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/logic/component/graphics/model.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

ModelComponent::ModelComponent(Entity* parent, const std::string& model, const std::string& texture)
    : Component(parent)
    , _SkeletonDebug(false)
{
    _Renderable = new ModelRenderable(parent->GetUid());
    _Renderable->SetModel(model);
    _Renderable->SetTexture(texture);
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(Entity::MessageHandler(this, &ModelComponent::OnTransformChanged));
    
    UpdateTransform();
}

ModelComponent::~ModelComponent()
{
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(Entity::MessageHandler(this, &ModelComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid ModelComponent::GetType()
{
    return GetStaticType();
}

Uid ModelComponent::GetStaticType()
{
    return TypeHash("pb::ModelComponent");
}

void ModelComponent::SetShader(Shader* shader)
{
    _Renderable->SetShader(shader);
}

void ModelComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void ModelComponent::SetModel(const std::string& model)
{
    _Renderable->SetModel(model);
}

const std::string& ModelComponent::GetModel()
{
    return _Renderable->GetModel();
}

void ModelComponent::SetTexture(const std::string& texture)
{
    _Renderable->SetTexture(texture);
}

const std::string& ModelComponent::GetTexture()
{
    return _Renderable->GetTexture();
}

void ModelComponent::SetTint(const glm::vec4& tint)
{
    _Renderable->SetTint(tint);
}

void ModelComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void ModelComponent::SetAnimation(const std::string& animation)
{
    _Animation = animation;
}

void ModelComponent::SetAnimationTime(float time)
{
    _AnimationTime = time;
}

void ModelComponent::SetSkeletonDebug(bool debug)
{
    if (debug == _SkeletonDebug)
        return;
    
    _SkeletonDebug = debug;
    
    if (debug)
    {
        GetParent()->RegisterMessageHandler<pb::DebugRenderMessage>(Entity::MessageHandler(this, &ModelComponent::OnDebugRender));
    } else {
        GetParent()->UnregisterMessageHandler<pb::DebugRenderMessage>(Entity::MessageHandler(this, &ModelComponent::OnDebugRender));
    }
}

glm::mat4x4 ModelComponent::GetBoneTransform(const ModelBone& bone, const ModelAnimation& animation, int frame)
{
    pb::Model* model = pb::Engine::Instance()->GetModelRenderer()->GetModel(_Renderable->GetModel());
    
    glm::mat4x4 transform = animation._Frames[frame][bone._Id];
    if (bone._ParentId != -1)
        transform = GetBoneTransform(model->GetDefinition()->Bones[bone._ParentId], animation, frame) * transform;
    
    return transform;
}

void ModelComponent::OnDebugRender(const pb::Message& message)
{
    const pb::DebugRenderMessage& debugMessage = static_cast<const pb::DebugRenderMessage&>(message);
    
    pb::Model* model = pb::Engine::Instance()->GetModelRenderer()->GetModel(_Renderable->GetModel());
    
    glm::mat4x4 transformMatrix = GetParent()->GetComponentByType<pb::TransformComponent>()->GetMatrix() * _LocalTransform;
    
    pb::ModelAnimation& animation = model->GetDefinition()->Animations[0];
    int frame = glm::mod(_AnimationTime, animation._Length)*(float)animation._FPS;
    
    for (std::vector<pb::ModelBone>::iterator it = model->GetDefinition()->Bones.begin(); it != model->GetDefinition()->Bones.end(); ++it)
    {
        if (it->_ParentId != -1)
        {
            glm::mat4x4 boneTransform = GetBoneTransform(*it, animation, frame);
            glm::mat4x4 parentBoneTransform = GetBoneTransform(model->GetDefinition()->Bones[it->_ParentId], animation, frame);
            
            glm::vec4 posA = transformMatrix * boneTransform * glm::vec4(0,0,0,1);
            glm::vec4 posB = transformMatrix * parentBoneTransform * glm::vec4(0,0,0,1);
            
            debugMessage.GetDebugRenderSystem()->AddLine(pb::kRenderPassScene, 10, glm::vec3(posA.x, posA.y, posA.z), glm::vec3(posB.x, posB.y, posB.z), glm::vec4(it->_ParentId * 0.2, it->_ParentId * 0.2, it->_ParentId * 0.2, 1));
        }
    }
}

void ModelComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void ModelComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    }
}

