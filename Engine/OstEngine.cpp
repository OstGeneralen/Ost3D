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
#include <Engine/Utility/Logging/Logging.h>

ost::dx::RenderingBackend g_renderBackend;

#if ENGINE_GUI_ENABLED
#include <Engine/Editor/EngineGUI/OnscreenLogViewer.h>
#include <Engine/Editor/EngineGUI/FrameDetailsGUI.h>
ost::editor::GUIHandler g_guiHandler;

ost::gui::OnscreenLogViewer g_onscreenLogViewer(8);
ost::gui::FrameDetailsGUI g_frameDetailsGUI;
#endif

STATIC_LOG(EngineLog);

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

	log::LoggingContext::Uninitialize();
}

void OstEngine::CreateAppWindow(const wchar_t* title, Dimensions windowDimensions)
{
	log::LoggingContext::Initialize();

	_appWindow.Create(title, windowDimensions);
	g_renderBackend.InitializeForWindow(_appWindow);

#if ENGINE_GUI_ENABLED
	g_guiHandler.Init(_appWindow, g_renderBackend);
	g_onscreenLogViewer.Initialize();
	g_frameDetailsGUI.BindFPSTracker(_fpsTracker);
#endif

	EngineLog.LOG_CONFIRM("OstEngine Initialized");
}

bool ost::OstEngine::IsAppWindowOpen() const
{
	return _appWindow.GetIsOpen();
}

void ost::OstEngine::BeginFrame()
{
	float deltaTime = 0.0f;

	if (_frameTimer.IsRunning())
	{
		_frameTimer.Stop();
		deltaTime = _frameTimer.GetDurationSeconds();
		_fpsTracker.AddSample(deltaTime);
		_fpsTracker.TickTracker();
	}
	_frameTimer.Start();


	_appWindow.ProcessEvents();
	g_renderBackend.BeginFrame(RGBAColor_f32(0x101010FF));

#if ENGINE_GUI_ENABLED
	g_guiHandler.BeginGuiFrame();
	g_onscreenLogViewer.Display();
	g_frameDetailsGUI.Display();

	ImGui::Begin("Render Info", 0, ImGuiWindowFlags_NoSavedSettings);
	g_renderBackend.DisplayGUI();
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


void ost::OstEngine::Shutdown()
{
	g_guiHandler.Uninit();
	g_renderBackend.Release();
}