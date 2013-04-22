#include <map>

#include "pixelboost/graphics/renderer/text/textRenderer.h"
#include "pixelboost/logic/system/graphics/render/render.h"

namespace pb
{
    
    class TextRenderable;
    class PrimitiveRenderableEllipse;
    class PrimitiveRenderableLine;
    class PrimitiveRenderableRectangle;
    class Renderable;

    class DebugRenderSystem : public RenderSystem
    {
    public:
        DebugRenderSystem();
        virtual ~DebugRenderSystem();
        
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        virtual void Update(Scene* scene, float totalTime, float gameTime);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);

        virtual const std::set<Renderable*>& GetItems(RenderPass pass);
        
        PrimitiveRenderableEllipse* AddEllipse(RenderPass renderPass, int layer, glm::vec3 position, glm::vec3 rotation, glm::vec2 size, glm::vec4 color = glm::vec4(1,1,1,1), float time = 0.f);
        PrimitiveRenderableLine* AddLine(RenderPass renderPass, int layer, glm::vec3 start, glm::vec3 end, glm::vec4 color = glm::vec4(1,1,1,1), float time = 0.f);
        PrimitiveRenderableRectangle* AddRectangle(RenderPass renderPass, int layer, glm::vec3 position, glm::vec3 rotation, glm::vec2 size, glm::vec4 color = glm::vec4(1,1,1,1), float time = 0.f);
        TextRenderable* AddText(RenderPass renderPass, int layer, FontAlign align, const std::string font, const std::string text, glm::vec3 position, glm::vec3 rotation, float size, float time = 0.f);
        
    private:
        void AddTimedItem(Renderable* renderable, float time);
        virtual void RemoveItem(Renderable* renderable);

        void Clear();
        
        std::set<Renderable*> _SceneRenderables;
        std::set<Renderable*> _UiRenderables;
        std::map<Renderable*, float> _Time;
        
        float _UpdateTime;
    };
    
}
