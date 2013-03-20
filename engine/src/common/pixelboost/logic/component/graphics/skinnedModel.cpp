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
    : RenderableComponent<pb::ModelRenderable>(parent)
    , _SkeletonDebug(false)
    , _AnimationState(0)
{
    GetEntity()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &SkinnedModelComponent::OnUpdate));
}

SkinnedModelComponent::~SkinnedModelComponent()
{
    if (_SkeletonDebug)
    {
        GetEntity()->UnregisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &SkinnedModelComponent::OnDebugRender));
    }
    
    GetEntity()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &SkinnedModelComponent::OnUpdate));

    delete _AnimationState;
}

void SkinnedModelComponent::SetShader(Shader* shader)
{
    GetRenderable()->SetShader(shader);
}

void SkinnedModelComponent::SetLayer(int layer)
{
    GetRenderable()->SetLayer(layer);
}

void SkinnedModelComponent::SetModel(Model* model)
{
    GetRenderable()->SetModel(model);
    _AnimationState = new SkinnedAnimationState(model->GetDefinition());
}

Model* SkinnedModelComponent::GetModel()
{
    return GetRenderable()->GetModel();
}

void SkinnedModelComponent::SetTexture(Texture* texture)
{
    GetRenderable()->SetTexture(texture);
}

Texture* SkinnedModelComponent::GetTexture()
{
    return GetRenderable()->GetTexture();
}

void SkinnedModelComponent::SetTint(const glm::vec4& tint)
{
    GetRenderable()->SetTint(tint);
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
    auto debugMessage = message.As<DebugRenderMessage>();
    
    Model* model = GetRenderable()->GetModel();
    
    if (!model)
        return;
    
    glm::mat4x4 transformMatrix = GetEntity()->GetComponent<TransformComponent>()->GetMatrix() * GetLocalTransform();
    
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

void SkinnedModelComponent::OnUpdate(const Message& message)
{
    auto updateMessage = message.As<UpdateMessage>();
    
    _AnimationState->AdvanceAnimation(updateMessage.GetGameDelta());
    _AnimationState->SoftwareSkin(GetRenderable()->GetModel());
}
