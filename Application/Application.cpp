#include "Application.h"

Application::Application(const AppRuntimeContext& context, int winW, int winH, const wchar_t* winT)
	: _window{ static_cast<HINSTANCE>(context.GetInstanceHandle()), context.GetCmdLineOpt(), winW, winH, winT }
{
}

void Application::Run()
{
	while (_window.GetIsOpen())
	{
		_window.ProcessEvents();
	}
}
