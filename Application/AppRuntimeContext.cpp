#include "AppRuntimeContext.h"

AppRuntimeContext::AppRuntimeContext()
    : _instanceHandlePtr(nullptr)
    , _cmdLineOpt(0)
{
}

AppRuntimeContext::AppRuntimeContext(void* instanceHandle, int commandLineOptions)
    : _instanceHandlePtr(instanceHandle)
    , _cmdLineOpt(commandLineOptions)
{
}

void* AppRuntimeContext::GetInstanceHandle() const
{
    return _instanceHandlePtr;
}

int AppRuntimeContext::GetCmdLineOpt() const
{
    return _cmdLineOpt;
}
