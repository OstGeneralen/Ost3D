#pragma once
#include "Application/AppRuntimeContext.h"
#include <Graphics/Window/Window.h>

class Application
{
public:
	Application( const AppRuntimeContext& context, int winW, int winH, const wchar_t* winT );
	Application() = delete;
	Application(const Application&) = delete;

public:

	void Run();
private:
	Window _window;
};