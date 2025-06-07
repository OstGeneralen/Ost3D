#pragma once
#if _WIN32
#include <Windows.h>
#endif

#include <Engine/OstEngineApp.h>

namespace ost
{
	extern void Ost3D_Run_Win32(OstEngineApp& appInstance, HINSTANCE windowsInstance, LPSTR args, int cmdShow );
}