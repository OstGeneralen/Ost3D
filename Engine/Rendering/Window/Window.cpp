#include "Window.h"
#include <Windows.h>
#include <unordered_map>
#include <Engine/Rendering/WindowsInfo.h>

std::unordered_map<void*, ost::Window*> WindowHandleClassMap;
const LPCWSTR WndClassName = L"OstWindowClass";

using namespace ost;

LRESULT CALLBACK WindowProcedure(HWND winHandle, UINT message, WPARAM wParam, LPARAM  lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		WindowHandleClassMap[winHandle]->Close();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(winHandle, message, wParam, lParam);
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

	ShowWindow(_windowHandle, internal::WindowsInfo::cmdShow);
	WindowHandleClassMap.insert({ _windowHandle, this });
	_open = true;

	_windowDimensions = windowDimensions;
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

	while(PeekMessage(&msg, _windowHandle, 0, 0, TRUE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
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
