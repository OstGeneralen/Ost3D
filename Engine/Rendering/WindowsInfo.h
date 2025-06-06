#pragma once
#include <Windows.h>

namespace ost
{
	namespace internal
	{
		struct WindowsInfo
		{
			static HINSTANCE appInstance;
			static LPSTR cmdLine;
			static int cmdShow;
		};
	}
}