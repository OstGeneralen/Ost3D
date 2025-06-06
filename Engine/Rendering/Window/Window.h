#pragma once
#include <Windows.h>
#include <Engine/Math/Vector/VectorsGeneric.h>
#include <Engine/Rendering/Window/WindowEventListener.h>
#include <unordered_set>

namespace ost
{
	class Window
	{
	public:
		void Create(const wchar_t* title, Dimensions windowDimensions);

		void RegisterEventListener(WindowEventListener* listener);
		void RemoveEventListener(WindowEventListener* listener);

		bool GetIsOpen() const;

		void Close();
		void ProcessEvents();

		void NotifyResizeOrMoveStart();
		void NotifyResizeOrMoveEnd();

		void OnResize();

		HWND GetHWND() const;

		const Dimensions& GetDimensions() const;

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

	private:
		std::unordered_set<WindowEventListener*> _eventListeners;
		HWND _windowHandle;
		Dimensions _windowDimensions;
		bool _open;

		bool _activeResize{ false };
	};
}