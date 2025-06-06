#pragma once

namespace ost
{
	class Window;
	namespace dx
	{
		class RenderingBackend;
	}

	namespace editor
	{
		class GUIHandler
		{
		public:
			void Init( const Window& appWindow, const dx::RenderingBackend& renderBackend );
			void Uninit();
			void BeginGuiFrame();
			void EndGuiFrame(const dx::RenderingBackend& renderBackend);
		private:

		};
	}
}