#pragma comment(lib, "Engine_Debug")

#include <Windows.h>
#include <Engine/OstEngine.h>

int WINAPI WinMain(
	_In_ HINSTANCE instanceHandle,
	_In_opt_ HINSTANCE previousInstanceHandle,
	_In_ LPSTR commandLine,
	_In_ int commandLineShow)
{
	ost::OstEngine engine(instanceHandle, commandLine, commandLineShow);
	engine.CreateAppWindow(L"OstEngine Window", { 1920, 1080 });
	
	while (engine.IsAppWindowOpen())
	{
		engine.BeginFrame();
		engine.EndFrame();
	}

	return 0;
}