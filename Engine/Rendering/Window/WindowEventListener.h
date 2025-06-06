#pragma once
#include <Engine/Math/Vector/VectorsGeneric.h>
#include <functional>

namespace ost
{
	struct WindowEventListener
	{
		void NotifySizeChange(Dimensions newDimensions) { if (ResizeCallback) ResizeCallback(newDimensions); }

		std::function<void(Dimensions)> ResizeCallback;
	};
}