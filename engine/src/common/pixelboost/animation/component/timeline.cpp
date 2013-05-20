#include "pixelboost/animation/component/timeline.h"
#include "pixelboost/animation/message/timeline.h"
#include "pixelboost/animation/timeline/timeline.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::TimelineComponent)

TimelineComponent::TimelineComponent(Entity* parent)
    : Component(parent)
{
    _Timeline = new Timeline();
    _UseGlobalTime = false;
    _IsPlaying = false;
    
    GetEntity()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &TimelineComponent::OnUpdate));
}

TimelineComponent::~TimelineComponent()
{
    GetEntity()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &TimelineComponent::OnUpdate));
    
    delete _Timeline;
}

Timeline* TimelineComponent::GetTimeline()
{
    return _Timeline;
}

void TimelineComponent::Play()
{
    _Timeline->Play();
}

void TimelineComponent::SetUseGlobalTime(bool useGlobalTime)
{
    _UseGlobalTime = useGlobalTime;
}

void TimelineComponent::OnUpdate(const Message& message)
{
    auto updateMessage = message.As<UpdateMessage>();
    
    if (_Timeline->IsPlaying() && !_IsPlaying)
    {
        _IsPlaying = true;
        GetEntity()->SendMessage(TimelinePlayingMessage(GetEntity(), this));
    }
    
    _Timeline->Update(_UseGlobalTime ? updateMessage.GetTimeDelta() : updateMessage.GetGameDelta());
    
    if (!_Timeline->IsPlaying() && _IsPlaying)
    {
        GetEntity()->SendMessage(TimelineStoppedMessage(GetEntity(), this));
    }
}
