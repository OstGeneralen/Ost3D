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


private:
	HWND _windowHandle;
	bool _open;
};