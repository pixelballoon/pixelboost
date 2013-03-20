#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{

class IndexBuffer;
class SpriteSheet;
class VertexBuffer;
    
struct Sprite;

class SpriteRenderable : public Renderable
{
public:
    SpriteRenderable();
    ~SpriteRenderable();
    
    virtual Uid GetType();
    static Uid GetStaticType();
    
    virtual void CalculateBounds();
    virtual void CalculateWorldMatrix();
    
    virtual Shader* GetShader();
    
    Sprite* GetSprite();
    void SetSprite(Sprite* sprite);
    
    void SetTransform(const glm::mat4x4& transform);
    void SetTint(glm::vec4 tint);
    
private:
    pb::Sprite* _Sprite;
    glm::mat4x4 _Transform;
    glm::vec4 _Tint;
    
    friend class SpriteRenderer;
};

class SpriteRenderer : public IRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();
    
    static SpriteRenderer* Instance();
    
    void Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
		
private:
    void RenderBatch();
    
private:
    int _MaxBatchSize;
    int _BatchSize;
    IndexBuffer* _IndexBuffer;
    VertexBuffer* _VertexBuffer;
    
    static SpriteRenderer* _Instance;
    
    friend class SpriteSheet;
};
    
}
