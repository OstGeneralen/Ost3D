#pragma once
#include <Engine/OstEngineApp.h>

class Application : public ost::OstEngineApp
{
public:
	std::string GetAppName() const override { return "My Ost3D App"; }

	void Start() override;
	void Tick(float dt) override;
	void Shutdown() override;
};