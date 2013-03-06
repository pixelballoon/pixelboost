#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

GuiRenderSystem::GuiRenderSystem()
{
    
}

GuiRenderSystem::~GuiRenderSystem()
{
}

Uid GuiRenderSystem::GetType() const
{
    return GuiRenderSystem::GetStaticType();
}

Uid GuiRenderSystem::GetStaticType()
{
    return TypeHash("pb::GuiRenderSystem");
}

void GuiRenderSystem::Update(Scene* scene, float totalTime, float gameTime)
{
    scene->BroadcastMessage(GuiRenderMessage(this));
}

void GuiRenderSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{

}
