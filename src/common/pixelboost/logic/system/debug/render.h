#include <map>

#include "pixelboost/logic/system/graphics/render/render.h"

namespace pb
{
    
    class PrimitiveRenderableEllipse;
    class PrimitiveRenderableLine;
    class Renderable;

    class DebugRenderSystem : public RenderSystem
    {
    public:
        DebugRenderSystem();
        virtual ~DebugRenderSystem();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void Update(Scene* scene, float time);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
        PrimitiveRenderableEllipse* AddEllipse(RenderPass renderPass, glm::vec3 position, glm::vec3 rotation, glm::vec2 size, float time = 0.f);
        PrimitiveRenderableLine* AddLine(RenderPass renderPass, glm::vec3 start, glm::vec3 end, float time = 0.f);
        
    private:
        virtual void AddItem(Renderable* renderable, float time);
        virtual void RemoveItem(Renderable* renderable);

        void Clear();
        
        typedef std::map<Renderable*, float> RenderableMap;
        
        RenderableMap _SceneRenderables;
        RenderableMap _UiRenderables;
        
        float _UpdateTime;
    };
    
}
