#pragma once

namespace pb
{
    enum ResourceThread
    {
        kResourceThreadMain,
        kResourceThreadAny,
    };
    
    enum ResourceReadyState
    {
        kResourceReadyStateReady,
        kResourceReadyStateAwaitingDependencies,
        kResourceReadyStateError,
    };
    
    enum ResourceProcess
    {
        kResourceProcessLoad,
        kResourceProcessProcess,
        kResourceProcessPostProcess,
        kResourceProcessUnload,
    };
    
    enum ResourceState
    {
        kResourceStateCreated,
        kResourceStateLoading,
        kResourceStateProcessing,
        kResourceStatePostProcessing,
        kResourceStateReady,
        kResourceStateError,
        kResourceStateUnloading,
    };
    
    enum ResourceError
    {
        kResourceErrorNone,
        kResourceErrorNoSuchResource,
        kResourceErrorSystemError,
        kResourceErrorUnknown,
    };
}
