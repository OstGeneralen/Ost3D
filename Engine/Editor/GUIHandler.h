#pragma once
#include <Engine/Rendering/Window/Window.h>
#include <Engine/EngineConfig.h>

namespace ost
{
	namespace dx
	{
		class RenderingBackend;
	}

	namespace gui
	{
		class GUIHandler
		{
		public:
#if ENGINE_GUI_ENABLED
			void Init( Window& appWindow, const dx::RenderingBackend& renderBackend );
			void Uninit();
			void BeginGuiFrame();
			void EndGuiFrame(const dx::RenderingBackend& renderBackend);
#else
			void Init(Window& appWindow, const dx::RenderingBackend& renderBackend) {}
			void Uninit() {}
			void BeginGuiFrame() {}
			void EndGuiFrame(const dx::RenderingBackend& renderBackend) {}
#endif
		private:
			void ProcessWindowResize(const Dimensions& newSize);

			Dimensions _windowDimensions;

			HEventListener _windowResizeListener;
		};
	}
}