#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class Effect;
    class Message;
    class ModelAnimationDefinition;
    class ModelBoneDefinition;
    class ModelRenderable;
    class SkinnedAnimationState;
    
    class SkinnedModelComponent : public RenderableComponent<ModelRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        SkinnedModelComponent(Entity* parent);
        virtual ~SkinnedModelComponent();
        
    public:
        void SetShader(Shader* shader);
        void SetLayer(int layer);
        
        void SetModel(Model* model);
        Model* GetModel();
        
        void SetTint(const glm::vec4& tint);
        const glm::vec4& GetTint();
        
        void SetAnimation(const std::string& animation);
        
        void SetSkeletonDebug(bool debug);
        
    private:
        void OnDebugRender(const Message& message);
        void OnUpdate(const Message& message);

        SkinnedAnimationState* _AnimationState;
        
        bool _SkeletonDebug;
    };
    
}
