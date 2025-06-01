#include "Application.h"
#include "DirectX/DXHandler.h"

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")

Application::Application(const AppRuntimeContext& context, int winW, int winH, const wchar_t* winT)
	: _window{ static_cast<HINSTANCE>(context.GetInstanceHandle()), context.GetCmdLineOpt(), winW, winH, winT }
{
}

void Application::Run()
{
	DXHandler dxHnd;

	DXHandlerInitErrorFlag successFlag = dxHnd.Initialize(_window);

	if (successFlag != DXHandlerErrors::Success)
	{
		// Whaaaat?
		dxHnd.Release();
		return;
	}

	while (_window.GetIsOpen())
	{
		_window.ProcessEvents();
	}

	dxHnd.Release();
}
