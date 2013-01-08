#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Effect;
    class Message;
    class ModelAnimationDefinition;
    class ModelBoneDefinition;
    class ModelRenderable;
    class SkinnedAnimationState;
    
    class SkinnedModelComponent : public Component
    {
    public:
        SkinnedModelComponent(Entity* parent, Model* model, Texture* texture);
        virtual ~SkinnedModelComponent();
        
        Uid GetType();
        static Uid GetStaticType();
        
        void SetShader(Shader* shader);
        void SetLayer(int layer);
        
        void SetModel(Model* model);
        Model* GetModel();
        
        void SetTexture(Texture* texture);
        Texture* GetTexture();
        
        void SetTint(const glm::vec4& tint);
        
        void SetLocalTransform(const glm::mat4x4& transform);

        void SetAnimation(const std::string& animation);
        
        void SetSkeletonDebug(bool debug);
        
    private:
        void OnDebugRender(const Message& message);
        void OnTransformChanged(const Message& message);
        void OnUpdate(const Message& message);
        void UpdateTransform();
        
        glm::mat4x4 _LocalTransform;
        ModelRenderable* _Renderable;
        
        SkinnedAnimationState* _AnimationState;
        
        bool _SkeletonDebug;
    };
    
}
