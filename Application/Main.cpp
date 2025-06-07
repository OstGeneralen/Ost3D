#pragma comment(lib, "Engine_Debug")

#include <Windows.h>
#include <Engine/Ost3D.h>
#include "Application.h"

int WINAPI WinMain(
	_In_ HINSTANCE instanceHandle,
	_In_opt_ HINSTANCE previousInstanceHandle,
	_In_ LPSTR commandLine,
	_In_ int commandLineShow)
{
	Application app;
	ost::Ost3D_Run_Win32(app, instanceHandle, commandLine, commandLineShow);
	return 0;
}