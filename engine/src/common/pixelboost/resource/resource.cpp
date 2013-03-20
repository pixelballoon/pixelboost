#include "pixelboost/resource/resource.h"

using namespace pb;

Resource::Resource(ResourcePool* pool, const std::string& filename)
: _Pool(pool)
, _Filename(filename)
, _State(kResourceStateLoading)
, _Error(kResourceErrorNone)
{
    
}

Resource::~Resource()
{
    
}

void Resource::Load()
{
    _Pool->LoadResource(_Filename);
}

void Resource::Unload()
{
    _Pool->UnloadResource(_Filename);
}

ResourceState Resource::GetState()
{
    return _State;
}

ResourceError Resource::GetError()
{
    return _Error;
}

const std::string& Resource::GetErrorDetails()
{
    return _ErrorDetails;
}

ResourceReadyState Resource::IsReadyToProcess(ResourceProcess process, std::string& errorDetails)
{
    return kResourceReadyStateReady;
}

ResourceThread Resource::GetResourceThread(ResourceProcess process)
{
    return kResourceThreadAny;
}

ResourceReadyState Resource::IsReadyToProcess()
{
    return IsReadyToProcess(GetProcessForState(_State), _ErrorDetails);
}

void Resource::Process()
{
    _Error = ProcessResource(_Pool, GetProcessForState(_State), _Filename, _ErrorDetails);
    
    if (_Error == kResourceErrorNone)
    {
        switch (_State)
        {
            case kResourceStateLoading:
                SetState(kResourceStateProcessing);
                break;
            case kResourceStateProcessing:
                SetState(kResourceStatePostProcessing);
                break;
            case kResourceStatePostProcessing:
                SetState(kResourceStateReady);
                break;
            case kResourceStateUnloading:
                SetState(kResourceStateCreated);
                break;
            case kResourceStateCreated:
            case kResourceStateReady:
            case kResourceStateError:
                PbAssert(!"Processing a resource in this state should never occur");
                break;
                
        }
    } else {
        SetState(kResourceStateError);
    }
}

ResourceThread Resource::GetThread(ResourceProcess process)
{
    return GetResourceThread(process);
}

ResourceProcess Resource::GetProcessForState(ResourceState state)
{
    switch (state)
    {
        case kResourceStateLoading:
            return kResourceProcessLoad;
        case kResourceStateProcessing:
            return kResourceProcessProcess;
        case kResourceStatePostProcessing:
            return kResourceProcessPostProcess;
        case kResourceStateUnloading:
            return kResourceProcessUnload;
        default:
        {
            PbAssert(!"We should never be processing in this state!");
            return kResourceProcessLoad;
        }
    }
}

void Resource::SetState(ResourceState state)
{
    _State = state;
    
    if (_State == kResourceStateReady || _State == kResourceStateError)
    {
        SignalResourceLoaded(this, _State == kResourceStateError);
    }
    
    if (_State == kResourceStateUnloading)
    {
        SignalResourceUnloading(this);
    }
}
