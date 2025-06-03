#include <Windows.h>
#include "Application/Application.h"
#include "Application/AppRuntimeContext.h"

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")

int WINAPI WinMain(
	_In_ HINSTANCE instanceHandle,
	_In_opt_ HINSTANCE previousInstanceHandle,
	_In_ LPSTR commandLine,
	_In_ int commandLineShow)
{
	AppRuntimeContext runtimeContext{instanceHandle, commandLineShow};
	Application app(runtimeContext, 800, 800, L"Test Window");
	app.Run();
	return 0;
}