#include "pixelboost/debug/log.h"
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

const std::string& Resource::GetFilename()
{
    return _Filename;
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
    switch (_State)
    {
        case kResourceStateLoading:
            PbLogInfo("pb.resource", "Loading resource (%s)", _Filename.c_str());
            break;
        case kResourceStateProcessing:
            PbLogInfo("pb.resource", "Processing resource (%s)", _Filename.c_str());
            break;
        case kResourceStatePostProcessing:
            PbLogInfo("pb.resource", "Post-processing resource (%s)", _Filename.c_str());
            break;
        case kResourceStateUnloading:
            PbLogInfo("pb.resource", "Unloading resource (%s)", _Filename.c_str());
            break;
        case kResourceStateCreated:
        case kResourceStateReady:
        case kResourceStateError:
            PbAssert(!"Processing a resource in this state should never occur");
            return;
    }
    
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
    
    ResourceManager::Instance()->AddStateChange(this);
}

void Resource::NotifyStateChange()
{
    if (_State == kResourceStateReady || _State == kResourceStateError)
    {
        SignalResourceLoaded(this, _State == kResourceStateError);
    }
    
    if (_State == kResourceStateUnloading)
    {
        SignalResourceUnloading(this);
    }
}
