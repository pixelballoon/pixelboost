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
    
    enum ResourceState
    {
        kResourceStateLoading,
        kResourceStateProcessing,
        kResourceStatePostProcessing,
        kResourceStateComplete,
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
