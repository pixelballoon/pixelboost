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
    class ModelAnimation;
    class ModelBone;
    class ModelRenderable;
    
    class ModelComponent : public Component
    {
    public:
        ModelComponent(Entity* parent, const std::string& model, const std::string& texture);
        virtual ~ModelComponent();
        
        Uid GetType();
        static Uid GetStaticType();
        
        void SetShader(Shader* shader);
        void SetLayer(int layer);
        
        void SetModel(const std::string& model);
        const std::string& GetModel();
        
        void SetTexture(const std::string& texture);
        const std::string& GetTexture();
        
        void SetTint(const glm::vec4& tint);
        
        void SetLocalTransform(const glm::mat4x4& transform);

        void SetAnimation(const std::string& animation);
        void SetAnimationTime(float time);
        
        void SetSkeletonDebug(bool debug);
        
    private:
        glm::mat4x4 GetBoneTransform(const ModelBone& bone, const ModelAnimation& animation, int frame);
        void OnDebugRender(const Message& message);
        void OnTransformChanged(const Message& message);
        void UpdateTransform();
        
        glm::mat4x4 _LocalTransform;
        ModelRenderable* _Renderable;
        
        std::string _Animation;
        float _AnimationTime;
        
        bool _SkeletonDebug;
    };
    
}
