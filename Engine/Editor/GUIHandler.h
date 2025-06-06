#pragma once
#include <Engine/Rendering/Window/Window.h>

namespace ost
{
	namespace dx
	{
		class RenderingBackend;
	}

	namespace editor
	{
		class GUIHandler
		{
		public:
			void Init( Window& appWindow, const dx::RenderingBackend& renderBackend );
			void Uninit();
			void BeginGuiFrame();
			void EndGuiFrame(const dx::RenderingBackend& renderBackend);
		private:
			void ProcessWindowResize(const Dimensions& newSize);

			Dimensions _windowDimensions;

			HEventListener _windowResizeListener;
		};
	}
}