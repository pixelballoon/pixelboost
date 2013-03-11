#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::GuiComponent)

GuiComponent::GuiComponent(Entity* parent)
    : RenderableComponent<GuiRenderable>(parent)
{
    SetRenderPass(pb::kRenderPassUi);
    
    GetScene()->GetSystemByType<GuiRenderSystem>()->AddGui(this);
}

GuiComponent::~GuiComponent()
{
	GetScene()->GetSystemByType<GuiRenderSystem>()->RemoveGui(this);
}

void GuiComponent::Render(GuiRenderSystem* system)
{
    GetEntity()->SendMessage(GuiRenderMessage(system, this));
}
