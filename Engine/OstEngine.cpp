#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")

#include "OstEngine.h"
#if _WIN32
#include "Rendering/WindowsInfo.h"
#endif
#include <Engine/Rendering/DX/RenderingBackend.h>

ost::dx::RenderingBackend g_renderBackend;

using namespace ost;

#if _WIN32
ost::OstEngine::OstEngine(HINSTANCE appHinstance, LPSTR cmd, int cmdShow)
{
	internal::WindowsInfo::appInstance = appHinstance;
	internal::WindowsInfo::cmdLine = cmd;
	internal::WindowsInfo::cmdShow = cmdShow;
}
#endif

void OstEngine::CreateAppWindow(const wchar_t* title, Dimensions windowDimensions)
{
	_appWindow.Create(title, windowDimensions);
	g_renderBackend.InitializeForWindow(_appWindow);
}

bool ost::OstEngine::IsAppWindowOpen() const
{
	return _appWindow.GetIsOpen();
}

void ost::OstEngine::BeginFrame()
{
	_appWindow.ProcessEvents();
	g_renderBackend.BeginFrame(RGBAColor_f32(0x101010FF));
}

void ost::OstEngine::EndFrame()
{
	g_renderBackend.EndAndPresentFrame();
}
