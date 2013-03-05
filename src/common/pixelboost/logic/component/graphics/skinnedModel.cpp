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

PB_DEFINE_COMPONENT(pb::SkinnedModelComponent)

SkinnedModelComponent::SkinnedModelComponent(Entity* parent)
    : Component(parent)
    , _SkeletonDebug(false)
    , _AnimationState(0)
{
    _Renderable = new ModelRenderable(parent->GetUid());
    
    GetScene()->GetSystemByType<RenderSystem>()->AddItem(_Renderable);
    
    GetEntity()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &SkinnedModelComponent::OnTransformChanged));
    GetEntity()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &SkinnedModelComponent::OnUpdate));
    
    UpdateTransform();
}

SkinnedModelComponent::~SkinnedModelComponent()
{
    if (_SkeletonDebug)
    {
        GetEntity()->UnregisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &SkinnedModelComponent::OnDebugRender));
    }
    
    GetEntity()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &SkinnedModelComponent::OnTransformChanged));
    GetEntity()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &SkinnedModelComponent::OnUpdate));
    
    GetScene()->GetSystemByType<RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
    delete _AnimationState;
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
    _AnimationState = new SkinnedAnimationState(model->GetDefinition());
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
        GetEntity()->RegisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &SkinnedModelComponent::OnDebugRender));
    } else {
        GetEntity()->UnregisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &SkinnedModelComponent::OnDebugRender));
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
    const DebugRenderMessage& debugMessage = static_cast<const DebugRenderMessage&>(message);
    
    Model* model = _Renderable->GetModel();
    
    if (!model)
        return;
    
    glm::mat4x4 transformMatrix = GetEntity()->GetComponent<TransformComponent>()->GetMatrix() * _LocalTransform;
    
    for (std::vector<ModelBoneDefinition>::iterator it = model->GetDefinition()->Bones.begin(); it != model->GetDefinition()->Bones.end(); ++it)
    {
        if (it->_ParentId != -1)
        {
            glm::mat4x4 boneTransform = _AnimationState->GetBoneMatrix(it->_Id);
            glm::mat4x4 parentBoneTransform = _AnimationState->GetBoneMatrix(it->_ParentId);
            
            glm::vec4 posA = transformMatrix * boneTransform * glm::vec4(0,0,0,1);
            glm::vec4 posB = transformMatrix * parentBoneTransform * glm::vec4(0,0,0,1);
            
            debugMessage.GetDebugRenderSystem()->AddLine(kRenderPassScene, 10, glm::vec3(posA.x, posA.y, posA.z), glm::vec3(posB.x, posB.y, posB.z), glm::vec4(it->_ParentId * 0.2, it->_ParentId * 0.2, it->_ParentId * 0.2, 1));
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
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    }
}

