#pragma once

#include "glm/glm.hpp"
#include "pixelboost/db/definitions.h"
#include "pixelboost/maths/boundingSphere.h"

namespace pb
{
 
class Camera;
class RenderSystem;
class Shader;
class Viewport;
    
enum RenderPass
{
    kRenderPassScene,
    kRenderPassUi,
};
    
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
    
    void CalculateModelViewMatrix(Viewport* viewport, Camera* camera);
    const glm::mat4x4& GetModelViewMatrix() const;
    
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
    Shader* _Shader;
};
    
}
