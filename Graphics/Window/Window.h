#pragma once
#include <Windows.h>
#include <Common/Math/VectorsGeneric.h>

class Window
{
public:
	Window(HINSTANCE instanceHandle, int cmdShw, const Dimensions& winDimensions, const wchar_t* title);
	~Window() = default;

	bool GetIsOpen() const;

	void Close();
	void ProcessEvents();

	HWND GetHWND() const;

	const Dimensions& GetDimensions() const;

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

private:
	HWND _windowHandle;
	Dimensions _windowDimensions;
	bool _open;
};