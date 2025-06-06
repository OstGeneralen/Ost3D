#include "FrameDetailsGUI.h"
#include <Engine/Editor/ImGui/imgui.h>
#include <Engine/Utility/Memory/MemUtils.h>

ost::gui::FrameDetailsGUI::FrameDetailsGUI()
{
	_regraphTrackerTimer.SetSignalFunction([&] {RecordFPS(); });
	_regraphTrackerTimer.Start();
}

void ost::gui::FrameDetailsGUI::BindFPSTracker(const FPSTracker& tracker)
{
	_fpsTracker = &tracker;
}

void ost::gui::FrameDetailsGUI::Display()
{
	_regraphTrackerTimer.Tick();

	float w = 300.0f;
	float h = ImGui::GetIO().DisplaySize.y;

	float x = ImGui::GetIO().DisplaySize.x - w;
	float y = 0.0f;

	ImGui::SetNextWindowSize({ w,h });
	ImGui::SetNextWindowPos({ x,y });
	ImGui::Begin("FrameDataWindow", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs);

	ImGui::Text("Frame Info");
	ImGui::Separator();
	ImGui::Text("FPS:");
	ImGui::SameLine();
	if (_fpsTracker == nullptr)
	{
		ImGui::Text("No bound tracker");
	}
	else
	{
		ImVec4 valueColor{ 0.0f, 0.0f, 0.0f, 1.0f };
		float fpsValue = _fpsTracker->GetCurrentFramesPerSeconds();
		if (fpsValue >= 150.0f) valueColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		if (fpsValue < 150.0f && fpsValue >= 60.0f) valueColor = { 0.5f, 1.0f, 0.0f, 1.0f };
		if (fpsValue < 60.0f && fpsValue >= 30.0f) valueColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		else if (fpsValue < 30.0f) valueColor = { 1.0f, 0.0f, 0.0f, 1.0f };

		ImGui::TextColored(valueColor, "%.2f", _fpsTracker->GetCurrentFramesPerSeconds());
	}

	ImGui::PlotLines("##FpsGraph", _fpsRecords, s_fpsRecordCount, 0, 0, 0.0f, 200.0f, {0.0f, 50});

	ImGui::End();

}

void ost::gui::FrameDetailsGUI::RecordFPS()
{
	if (_fpsTracker == nullptr) return;

	MemCopy(_fpsRecords, &_fpsRecords[1], sizeof(float) * (s_fpsRecordCount - 1));
	_fpsRecords[s_fpsRecordCount - 1] = _fpsTracker->GetCurrentFramesPerSeconds();
}
