#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/graphics/resources/shaderResource.h"
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
    
    GetScene()->GetSystemByType<GuiSystem>()->AddGui(this);
    
    _GeometryShader = ResourceManager::Instance()->GetPool("pixelboost")->GetResource<ShaderResource>("/shaders/pb_solidColor.shc");
    _GeometryShader->SignalResourceLoaded.Connect(this, &GuiComponent::OnResourceLoaded);
    _GeometryShader->SignalResourceUnloading.Connect(this, &GuiComponent::OnResourceUnloading);
    if (_GeometryShader->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_GeometryShader.get(), false);
    }
    
    _SpriteShader = ResourceManager::Instance()->GetPool("pixelboost")->GetResource<ShaderResource>("/shaders/pb_texturedColor.shc");
    _SpriteShader->SignalResourceLoaded.Connect(this, &GuiComponent::OnResourceLoaded);
    _SpriteShader->SignalResourceUnloading.Connect(this, &GuiComponent::OnResourceUnloading);
    if (_SpriteShader->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_SpriteShader.get(), false);
    }
    
    _TextShader = ResourceManager::Instance()->GetPool("pixelboost")->GetResource<ShaderResource>("/shaders/pb_texturedColor.shc");
    _TextShader->SignalResourceLoaded.Connect(this, &GuiComponent::OnResourceLoaded);
    _TextShader->SignalResourceUnloading.Connect(this, &GuiComponent::OnResourceUnloading);
    if (_TextShader->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_TextShader.get(), false);
    }
}

GuiComponent::~GuiComponent()
{
	GetScene()->GetSystemByType<GuiSystem>()->RemoveGui(this);
}

void GuiComponent::SetSize(glm::vec2 size)
{
    _Size = size;
}

glm::vec2 GuiComponent::GetSize()
{
    return _Size;
}

void GuiComponent::OnGui(GuiState& state, GuiSystem* system, GuiRenderMessage::EventType eventType)
{
    GetEntity()->SendMessage(GuiRenderMessage(state, system, this, eventType));
}

void GuiComponent::OnResourceLoaded(Resource* resource, bool error)
{
    if (error)
        return;
    
    if (resource == _GeometryShader.get())
    {
        GetRenderable()->SetGeometryShader(_GeometryShader->GetShader());
    }
    
    if (resource == _SpriteShader.get())
    {
        GetRenderable()->SetSpriteShader(_SpriteShader->GetShader());
    }
    
    if (resource == _TextShader.get())
    {
        GetRenderable()->SetTextShader(_TextShader->GetShader());
    }
}

void GuiComponent::OnResourceUnloading(Resource* resource)
{
    if (resource == _GeometryShader.get())
    {
        GetRenderable()->SetGeometryShader(0);
    }
    
    if (resource == _SpriteShader.get())
    {
        GetRenderable()->SetSpriteShader(0);
    }
    
    if (resource == _TextShader.get())
    {
        GetRenderable()->SetTextShader(0);
    }
}
