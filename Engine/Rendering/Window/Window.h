#pragma once
#include <Windows.h>
#include <Engine/Math/Vector/VectorsGeneric.h>
#include <Engine/Rendering/Window/WindowEventListener.h>
#include <unordered_set>
#include <Engine/Utility/Events/Event.h>

namespace ost
{
	class Window
	{
	public: // Window Listenable Events
		TEvent<Dimensions> Event_Resize;

	public:
		void Create(const wchar_t* title, Dimensions windowDimensions);

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
		HWND _windowHandle;
		Dimensions _windowDimensions;
		bool _open;
		bool _activeResize{ false };
	};
}