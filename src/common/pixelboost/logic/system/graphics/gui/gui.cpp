#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

GuiRenderSystem::GuiRenderSystem()
{
    Engine::Instance()->GetKeyboardManager()->AddHandler(this);
    Engine::Instance()->GetMouseManager()->AddHandler(this);
}

GuiRenderSystem::~GuiRenderSystem()
{
    Engine::Instance()->GetKeyboardManager()->RemoveHandler(this);
    Engine::Instance()->GetMouseManager()->RemoveHandler(this);
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
    
}

void GuiRenderSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{
    for (const auto& gui : _GuiItems)
    {
        gui->Render(this);
    }
}

bool GuiRenderSystem::DoButton(GuiComponent* component, glm::vec2 position, glm::vec2 size)
{
    component->GetRenderable()->RenderLine(position, position + glm::vec2(size.x, 0), glm::vec4(1,1,1,1));
    component->GetRenderable()->RenderLine(position, position + glm::vec2(0, size.y), glm::vec4(1,1,1,1));
    component->GetRenderable()->RenderLine(position + glm::vec2(0, size.y), position + glm::vec2(size.x, size.y), glm::vec4(1,1,1,1));
    component->GetRenderable()->RenderLine(position + glm::vec2(size.x, 0), position + glm::vec2(size.x, size.y), glm::vec4(1,1,1,1));
    
    return false;
}

void GuiRenderSystem::AddGui(GuiComponent* component)
{
    _GuiItems.insert(component);
}

void GuiRenderSystem::RemoveGui(GuiComponent* component)
{
    _GuiItems.erase(component);
}

int GuiRenderSystem::GetInputHandlerPriority()
{
    return 10;
}

bool GuiRenderSystem::OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    return false;
}

bool GuiRenderSystem::OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    return false;
}

bool GuiRenderSystem::OnMouseMove(glm::vec2 position)
{
    return false;
}

bool GuiRenderSystem::OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 delta)
{
    return false;
}

bool GuiRenderSystem::OnMouseZoom(glm::vec2 zoom)
{
    return false;
}

bool GuiRenderSystem::OnMouseRotate(float rotate)
{
    return false;
}

bool GuiRenderSystem::OnKeyDown(KeyboardKey key, ModifierKeys modifier, char character)
{
    return false;
}

bool GuiRenderSystem::OnKeyUp(KeyboardKey key, ModifierKeys modifier, char character)
{
    return false;
}
