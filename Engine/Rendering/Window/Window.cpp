#include "Window.h"
#include <Windows.h>
#include <unordered_map>
#include <Engine/Rendering/WindowsInfo.h>
#include <Engine/Editor/ImGui/imgui_impl_win32.h>

#include <Engine/Utility/Logging/Logging.h>

STATIC_LOG(WindowLog);

std::unordered_map<void*, ost::Window*> WindowHandleClassMap;
const LPCWSTR WndClassName = L"OstWindowClass";

using namespace ost;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProcedure(HWND winHandle, UINT message, WPARAM wParam, LPARAM  lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(winHandle, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_CLOSE:
		WindowHandleClassMap[winHandle]->Close();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_ENTERSIZEMOVE:
		WindowHandleClassMap[winHandle]->NotifyResizeOrMoveStart();
		return 0;
	case WM_EXITSIZEMOVE:
		WindowHandleClassMap[winHandle]->NotifyResizeOrMoveEnd();
		return 0;
	case WM_SIZE:
		if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		{
			WindowHandleClassMap[winHandle]->OnResize();
			return 0;
		}
		break;
	}

	return DefWindowProc(winHandle, message, wParam, lParam);
}

void Window::Create(const char* title, Dimensions windowDimensions)
{
	std::string cs{ title };
	std::wstring ws{ cs.begin(), cs.end() };
	Create(ws.c_str(), windowDimensions);
}
void Window::Create(const wchar_t* title, Dimensions windowDimensions)
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProcedure;
	windowClass.hInstance = internal::WindowsInfo::appInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.lpszClassName = WndClassName;

	RegisterClassEx(&windowClass);

	_windowHandle = CreateWindowEx(
		NULL,
		WndClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		300, 300,
		windowDimensions.X, windowDimensions.Y,
		NULL, NULL,
		internal::WindowsInfo::appInstance,
		NULL);

	WindowHandleClassMap.insert({ _windowHandle, this });
	ShowWindow(_windowHandle, internal::WindowsInfo::cmdShow);
	_open = true;
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// Assign window dimensions with DPI
	RECT logicalRect{};
	GetClientRect(_windowHandle, &logicalRect);
	auto winDpi = GetDpiForWindow(_windowHandle);

	int physical_width = (logicalRect.right - logicalRect.left) * (winDpi / 96);
	int physical_height = (logicalRect.bottom - logicalRect.top) * (winDpi / 96);

	_windowDimensions = { physical_width, physical_height };
}

bool Window::GetIsOpen() const { return _open; }

void Window::Close()
{
	_open = false;
	CloseWindow(_windowHandle);
}

void Window::ProcessEvents()
{
	MSG msg;

	while (PeekMessage(&msg, _windowHandle, 0, 0, TRUE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void ost::Window::NotifyResizeOrMoveStart()
{
	_activeResize = true;
}

void ost::Window::NotifyResizeOrMoveEnd()
{
	_activeResize = false;
	OnResize();
}

void ost::Window::OnResize()
{
	if (_activeResize) return;

	RECT logicalRect{};
	GetClientRect(_windowHandle, &logicalRect);
	auto winDpi = GetDpiForWindow(_windowHandle);

	int physical_width = (logicalRect.right - logicalRect.left) * (winDpi / 96);
	int physical_height = (logicalRect.bottom - logicalRect.top) * (winDpi / 96);

	if (_windowDimensions.X == physical_width && _windowDimensions.Y == physical_height)
	{
		// Don't invoke any events since there isn't actually any change
		return;
	}

	_windowDimensions = { physical_width, physical_height };

	WindowLog.LOG_INFO("Window resized. New physical dimensions: {}x{}", physical_width, physical_height);

	Event_Resize.Raise(_windowDimensions);
}

HWND Window::GetHWND() const
{
	return _windowHandle;
}

const Dimensions& Window::GetDimensions() const
{
	return _windowDimensions;
}

unsigned int Window::GetWidth() const
{
	return _windowDimensions.X;
}

unsigned int Window::GetHeight() const
{
	return _windowDimensions.Y;
}
