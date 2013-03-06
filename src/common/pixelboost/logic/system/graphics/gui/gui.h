#include <map>

#include "pixelboost/logic/system.h"

namespace pb
{
    
    class GuiRenderSystem : public SceneSystem
    {
    public:
        GuiRenderSystem();
        virtual ~GuiRenderSystem();
        
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        virtual void Update(Scene* scene, float totalTime, float gameTime);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
    private:
    };
    
}
