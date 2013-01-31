#include <map>

#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/system/graphics/render/render.h"

namespace pb
{
    
    class FontRenderable;
    class PrimitiveRenderableEllipse;
    class PrimitiveRenderableLine;
    class Renderable;

    class DebugRenderSystem : public RenderSystem
    {
    public:
        DebugRenderSystem();
        virtual ~DebugRenderSystem();
        
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        virtual void Update(Scene* scene, float time);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
        PrimitiveRenderableEllipse* AddEllipse(RenderPass renderPass, int layer, glm::vec3 position, glm::vec3 rotation, glm::vec2 size, glm::vec4 color = glm::vec4(1,1,1,1), float time = 0.f);
        PrimitiveRenderableLine* AddLine(RenderPass renderPass, int layer, glm::vec3 start, glm::vec3 end, glm::vec4 color = glm::vec4(1,1,1,1), float time = 0.f);
        FontRenderable* AddText(RenderPass renderPass, int layer, FontAlign align, const std::string font, const std::string text, glm::vec3 position, glm::vec3 rotation, float size, float time = 0.f);
        
    private:
        void AddTimedItem(Renderable* renderable, float time);
        virtual void RemoveItem(Renderable* renderable);

        void Clear();
        
        typedef std::map<Renderable*, float> RenderableMap;
        
        RenderableMap _SceneRenderables;
        RenderableMap _UiRenderables;
        
        float _UpdateTime;
    };
    
}
