#pragma once

#include "glm/glm.hpp"
#include "pixelboost/framework/definitions.h"
#include "pixelboost/graphics/definitions.h"
#include "pixelboost/maths/boundingSphere.h"
#include "pixelboost/resource/resourceManager.h"

namespace pb
{
 
    class Camera;
    class Material;
    class MaterialResource;
    class RenderSystem;
    class Shader;
    class Viewport;
        
    class Renderable
    {
    public:
        Renderable(Uid entityUid);
        Renderable(Uid entityUid, Shader* shader);
        virtual ~Renderable();
        
        void SetSystem(RenderSystem* system);
        
        Uid GetEntityUid();
        
        virtual Uid GetType() = 0;
        
        void SetRenderPass(RenderPass renderPass);
        RenderPass GetRenderPass();
        
        void SetLayer(int layer);
        int GetLayer();
        
        virtual void CalculateBounds() = 0;
        void DirtyBounds();
        void SetBounds(const BoundingSphere& bounds);
        const BoundingSphere& GetBounds();
        
        virtual void CalculateWorldMatrix() = 0;
        void DirtyWorldMatrix();
        void SetWorldMatrix(const glm::mat4x4& matrix);
        const glm::mat4x4& GetWorldMatrix();
        
        void CalculateModelViewMatrix(const glm::mat4x4& viewMatrix);
        const glm::mat4x4& GetModelViewMatrix() const;
        
        virtual Material* GetMaterial();
        void SetMaterial(std::shared_ptr<ResourceHandle<MaterialResource> > material);
        
        virtual Shader* GetShader();
        void SetShader(Shader* shader);
        
    private:
        void RefreshSystemBinding();
        
        bool _WorldMatrixDirty;
        glm::mat4x4 _WorldMatrix;
        
        bool _BoundsDirty;
        BoundingSphere _Bounds;
        
        glm::mat4x4 _ModelViewMatrix;
        
    private:
        RenderSystem* _System;
        Uid _EntityUid;
        int _Layer;
        RenderPass _RenderPass;
        std::shared_ptr<ResourceHandle<MaterialResource> > _Material;
        Shader* _Shader;
    };
    
}
