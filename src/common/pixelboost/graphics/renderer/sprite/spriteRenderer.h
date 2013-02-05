#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/misc/pointer.h"

namespace pb
{

class IndexBuffer;
class SpriteSheet;
class Sprite;
class VertexBuffer;

class SpriteRenderable : public Renderable
{
public:
    SpriteRenderable(Uid entityId);
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
    
    void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass);
	
    std::shared_ptr<SpriteSheet> CreateSpriteSheet(const std::string& name);
    bool LoadSpriteSheet(const std::string& name, const std::string& extension, bool createMips=false, bool hasPremultipliedAlpha = true);
    bool UnloadSpriteSheet(const std::string& name);
    
    std::shared_ptr<SpriteSheet> GetSpriteSheet(const std::string& sheetName) const;
    Sprite* GetSprite(const std::string& spriteName) const;
		
private:
    void RenderBatch();
    
    typedef std::map<std::string, Sprite*> SpriteMap;
    typedef std::map<std::string, std::shared_ptr<SpriteSheet> > SheetMap;
	
    SpriteMap _Sprites;
	SheetMap _SpriteSheets;
    
private:
    int _MaxBatchSize;
    int _BatchSize;
    IndexBuffer* _IndexBuffer;
    VertexBuffer* _VertexBuffer;
    
    friend class SpriteSheet;
};
    
}
