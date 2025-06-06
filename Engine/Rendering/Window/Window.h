#pragma once
#include <Windows.h>
#include <Engine/Math/Vector/VectorsGeneric.h>

namespace ost
{
	class Window
	{
	public:
		void Create(const wchar_t* title, Dimensions windowDimensions);

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
}