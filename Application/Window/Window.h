#pragma once
#include <Windows.h>

class Window
{
public:
	Window(HINSTANCE instanceHandle, int cmdShw, int w, int h, const wchar_t* title);
	~Window() = default;

	bool GetIsOpen() const;

	void Close();
	void ProcessEvents();

	HWND GetHWND() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

private:
	HWND _windowHandle;
	unsigned int _width, _height;
	bool _open;
};