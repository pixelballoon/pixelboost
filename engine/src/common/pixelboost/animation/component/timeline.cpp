#include "pixelboost/animation/component/timeline.h"
#include "pixelboost/animation/timeline/timeline.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::TimelineComponent)

TimelineComponent::TimelineComponent(Entity* parent)
    : Component(parent)
{
    _Timeline = new Timeline();
    
    GetEntity()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &TimelineComponent::OnUpdate));
}

TimelineComponent::~TimelineComponent()
{
    GetEntity()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &TimelineComponent::OnUpdate));
}

Timeline* TimelineComponent::GetTimeline()
{
    return _Timeline;
}

void TimelineComponent::Play()
{
    _Timeline->Play();
}

void TimelineComponent::OnUpdate(const Message& message)
{
    auto updateMessage = message.As<UpdateMessage>();
    
    _Timeline->Update(updateMessage.GetGameDelta());
}
