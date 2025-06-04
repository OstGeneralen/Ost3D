#include "Application.h"
#include <Graphics/Rendering/RenderingFramework.h>
#include <pix3.h>

// ------------------------------------------------------------

Application::Application(const AppRuntimeContext& context, int winW, int winH, const wchar_t* winT)
	: _window{ static_cast<HINSTANCE>(context.GetInstanceHandle()), context.GetCmdLineOpt(), {winW, winH} , winT }
{
}

void Application::Run()
{
	IRenderFramework* renderFramework = Rendering::CreateFramework();
	if (!renderFramework->Initialize(_window))
	{
		return;
	}

	IRenderer& renderer = renderFramework->GetRenderer();


	while (_window.GetIsOpen())
	{
		_window.ProcessEvents();
		renderer.BeginRenderFrame(RGBAColor_u8(50, 180, 255, 255));
		renderer.EndRenderFrame();
		renderer.PresentFrame();
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------