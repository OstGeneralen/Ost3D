#pragma once
#include <string>

namespace ost
{
	class OstEngineApp
	{
	public:
		virtual ~OstEngineApp() = default;

		virtual std::string GetAppName() const { return "Ost3D App"; }

		virtual void Start() {}
		virtual void Tick(float dt) {}
		virtual void Shutdown() {}
	};
}