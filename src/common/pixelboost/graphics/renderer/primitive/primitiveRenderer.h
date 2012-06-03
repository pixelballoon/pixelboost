#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/irenderer.h"

namespace pb
{
    
class IndexBuffer;
class Material;
class VertexBuffer;
    
class PrimitiveRenderer : public IRenderer
{
public:
    PrimitiveRenderer();
    ~PrimitiveRenderer();
    
    void Update(float time);
    void Render(RenderLayer* layer);
    
    void AttachEllipse(RenderLayer* layer, glm::vec2 position, glm::vec2 size, glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f));
    void AttachLine(RenderLayer* layer, glm::vec2 start, glm::vec2 end, glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f));
    void AttachBox(RenderLayer* layer, glm::vec2 position, glm::vec2 size, glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f), bool solid=true);
    
private:
    struct PrimitiveInstance
    {
        enum Type
        {
            kTypeEllipse,
            kTypeLine,
            kTypeBox,
        };
        
        Type type;
        glm::vec2 position;
        glm::vec2 size;
        glm::vec3 rotation;
        glm::vec4 color;
        bool solid;
    };
    
    typedef std::vector<PrimitiveInstance> ItemList;
    typedef std::map<RenderLayer*, ItemList> ItemListMap;
    
    ItemListMap _Items;
    
    Material* _Material;
    
    IndexBuffer* _EllipseIndexBuffer;
    VertexBuffer* _EllipseVertexBuffer;
    
    IndexBuffer* _LineIndexBuffer;
    VertexBuffer* _LineVertexBuffer;
    
    IndexBuffer* _BoxIndexBuffer;
    VertexBuffer* _BoxVertexBuffer;
};
    
}
