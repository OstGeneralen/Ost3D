#pragma once

struct AppRuntimeContext
{
public:
	AppRuntimeContext();
	AppRuntimeContext(void* instanceHandle, int commandLineOptions);

	void* GetInstanceHandle() const;
	int GetCmdLineOpt() const;
private:
	void* _instanceHandlePtr;
	int _cmdLineOpt;
};
