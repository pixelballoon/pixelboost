#pragma once

namespace pb
{
    enum ResourceThread
    {
        kResourceThreadMain,
        kResourceThreadAny,
    };
    
    enum ResourceState
    {
        kResourceStateLoading,
        kResourceStateProcessing,
        kResourceStatePostProcessing,
        kResourceStateComplete,
        kResourceStateError,
    };
    
    enum ResourceError
    {
        kResourceErrorNone,
        kResourceErrorNoSuchResource,
        kResourceErrorSystemError,
        kResourceErrorUnknown,
    };
}
