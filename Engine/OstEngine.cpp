#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")

#include <Engine/EngineConfig.h>
#include "OstEngine.h"
#if _WIN32
#include "Rendering/WindowsInfo.h"
#endif
#include <Engine/Rendering/DX/RenderingBackend.h>
#include <Engine/Editor/GUIHandler.h>
#include <Engine/Editor/ImGui/imgui.h>

ost::dx::RenderingBackend g_renderBackend;

#if ENGINE_GUI_ENABLED
ost::editor::GUIHandler g_guiHandler;
#endif

using namespace ost;

#if _WIN32
ost::OstEngine::OstEngine(HINSTANCE appHinstance, LPSTR cmd, int cmdShow)
{
	internal::WindowsInfo::appInstance = appHinstance;
	internal::WindowsInfo::cmdLine = cmd;
	internal::WindowsInfo::cmdShow = cmdShow;
}
#endif

ost::OstEngine::~OstEngine()
{
#if ENGINE_GUI_ENABLED
	g_guiHandler.Uninit();
#endif
	g_renderBackend.Release();
}

void OstEngine::CreateAppWindow(const wchar_t* title, Dimensions windowDimensions)
{
	_appWindow.Create(title, windowDimensions);
	g_renderBackend.InitializeForWindow(_appWindow);

#if ENGINE_GUI_ENABLED
	g_guiHandler.Init(_appWindow, g_renderBackend);
#endif
}

bool ost::OstEngine::IsAppWindowOpen() const
{
	return _appWindow.GetIsOpen();
}

void ost::OstEngine::BeginFrame()
{
	_appWindow.ProcessEvents();
	g_renderBackend.BeginFrame(RGBAColor_f32(0x101010FF));

#if ENGINE_GUI_ENABLED
	g_guiHandler.BeginGuiFrame();

	ImGui::Begin("This is a test", nullptr, ImGuiWindowFlags_NoSavedSettings);
	ImGui::Text("Heres some text for you");
	ImGui::Button("A button");
	ImGui::End();
#endif
}

void ost::OstEngine::EndFrame()
{
#if ENGINE_GUI_ENABLED
	g_guiHandler.EndGuiFrame(g_renderBackend);
#endif
	g_renderBackend.EndAndPresentFrame();
}
