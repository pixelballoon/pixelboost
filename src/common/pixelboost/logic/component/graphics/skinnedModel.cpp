#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/asset/model.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/logic/component/graphics/skinnedModel.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

SkinnedModelComponent::SkinnedModelComponent(Entity* parent, Model* model, Texture* texture)
    : Component(parent)
    , _SkeletonDebug(false)
{
    _Renderable = new ModelRenderable(parent->GetUid());
    _Renderable->SetModel(model);
    _Renderable->SetTexture(texture);
    
    _AnimationState = new SkinnedAnimationState(model->GetDefinition());
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &SkinnedModelComponent::OnTransformChanged));
    GetParent()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &SkinnedModelComponent::OnUpdate));
    
    UpdateTransform();
}

SkinnedModelComponent::~SkinnedModelComponent()
{
    if (_SkeletonDebug)
    {
        GetParent()->UnregisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &SkinnedModelComponent::OnDebugRender));
    }
    
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &SkinnedModelComponent::OnTransformChanged));
    GetParent()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &SkinnedModelComponent::OnUpdate));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
    delete _AnimationState;
}

Uid SkinnedModelComponent::GetType()
{
    return GetStaticType();
}

Uid SkinnedModelComponent::GetStaticType()
{
    return TypeHash("pb::SkinnedModelComponent");
}

void SkinnedModelComponent::SetShader(Shader* shader)
{
    _Renderable->SetShader(shader);
}

void SkinnedModelComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void SkinnedModelComponent::SetModel(Model* model)
{
    _Renderable->SetModel(model);
}

Model* SkinnedModelComponent::GetModel()
{
    return _Renderable->GetModel();
}

void SkinnedModelComponent::SetTexture(Texture* texture)
{
    _Renderable->SetTexture(texture);
}

Texture* SkinnedModelComponent::GetTexture()
{
    return _Renderable->GetTexture();
}

void SkinnedModelComponent::SetTint(const glm::vec4& tint)
{
    _Renderable->SetTint(tint);
}

void SkinnedModelComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void SkinnedModelComponent::SetAnimation(const std::string& animation)
{
    _AnimationState->SetAnimation(animation);
}

void SkinnedModelComponent::SetSkeletonDebug(bool debug)
{
    if (debug == _SkeletonDebug)
        return;
    
    _SkeletonDebug = debug;
    
    if (debug)
    {
        GetParent()->RegisterMessageHandler<pb::DebugRenderMessage>(MessageHandler(this, &SkinnedModelComponent::OnDebugRender));
    } else {
        GetParent()->UnregisterMessageHandler<pb::DebugRenderMessage>(MessageHandler(this, &SkinnedModelComponent::OnDebugRender));
    }
}

/*
glm::mat4x4 SkinnedModelComponent::GetBoneTransform(const ModelBoneDefinition& bone, const ModelAnimationDefinition& animation, int frame)
{
    pb::Model* model = _Renderable->GetModel();
    
    if (!model)
        return glm::mat4x4();
    
    glm::mat4x4 transform = animation._Frames[frame][bone._Id];
    if (bone._ParentId != -1)
        transform = GetBoneTransform(model->GetDefinition()->Bones[bone._ParentId], animation, frame) * transform;
    
    return transform;
}
*/

void SkinnedModelComponent::OnDebugRender(const Message& message)
{
    const pb::DebugRenderMessage& debugMessage = static_cast<const pb::DebugRenderMessage&>(message);
    
    pb::Model* model = _Renderable->GetModel();
    
    if (!model)
        return;
    
    glm::mat4x4 transformMatrix = GetParent()->GetComponentByType<pb::TransformComponent>()->GetMatrix() * _LocalTransform;
    
    for (std::vector<pb::ModelBoneDefinition>::iterator it = model->GetDefinition()->Bones.begin(); it != model->GetDefinition()->Bones.end(); ++it)
    {
        if (it->_ParentId != -1)
        {
            glm::mat4x4 boneTransform = _AnimationState->GetBoneMatrix(it->_Id);
            glm::mat4x4 parentBoneTransform = _AnimationState->GetBoneMatrix(it->_ParentId);
            
            glm::vec4 posA = transformMatrix * boneTransform * glm::vec4(0,0,0,1);
            glm::vec4 posB = transformMatrix * parentBoneTransform * glm::vec4(0,0,0,1);
            
            debugMessage.GetDebugRenderSystem()->AddLine(pb::kRenderPassScene, 10, glm::vec3(posA.x, posA.y, posA.z), glm::vec3(posB.x, posB.y, posB.z), glm::vec4(it->_ParentId * 0.2, it->_ParentId * 0.2, it->_ParentId * 0.2, 1));
        }
    }
}

void SkinnedModelComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void SkinnedModelComponent::OnUpdate(const Message& message)
{
    const UpdateMessage& updateMessage = static_cast<const UpdateMessage&>(message);
    
    _AnimationState->AdvanceAnimation(updateMessage.GetGameDelta());
    _AnimationState->SoftwareSkin(_Renderable->GetModel());
}

void SkinnedModelComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    }
}

