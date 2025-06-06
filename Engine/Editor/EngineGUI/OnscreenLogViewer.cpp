#include "OnscreenLogViewer.h"
#include <Engine/Utility/Logging/LoggingContext.h>
#include <Engine/Editor/ImGui/imgui.h>
#include <Engine/Utility/Logging/LogFormatting.h>

namespace
{
	std::string LogVerbosityToString(ost::log::ELevel v)
	{
		switch (v)
		{
		case ost::log::ELevel::Verbose: return "VERBOSE";
		case ost::log::ELevel::Message: return "MESSAGE";
		case ost::log::ELevel::Warning: return "WARNING";
		case ost::log::ELevel::Error: return "ERROR";
		case ost::log::ELevel::Confirmation: return "CONFIRM";
		}
		return "";
	}

	ImVec4 LogVerbosityToImColor(ost::log::ELevel v)
	{
		switch (v)
		{
		case ost::log::ELevel::Verbose: return		{ 1.0f, 1.0f, 1.0f, 0.8f};
		case ost::log::ELevel::Message: return		{ 1.0f, 1.0f, 1.0f, 0.9f };
		case ost::log::ELevel::Info: return			{ 0.0f, 1.0f, 1.0f, 0.9f };
		case ost::log::ELevel::Warning: return		{ 1.0f, 1.0f, 0.1f, 1.0f };
		case ost::log::ELevel::Error: return		{ 1.0f, 0.1f, 0.1f, 1.0f };
		case ost::log::ELevel::Confirmation: return { 0.0f, 1.0f, 0.0f, 1.0f };
		}
		return { 1.f, 1.f, 1.f, 1.f };
	}

	ImVec4 s_lowFocusColor{ 0.8f, 0.8f, 0.8f, 1.0f };
}

ost::gui::OnscreenLogViewer::OnscreenLogViewer(uint32 visibleMessages)
	: _messageCount(visibleMessages)
{
}

ost::gui::OnscreenLogViewer::~OnscreenLogViewer()
{
}

void ost::gui::OnscreenLogViewer::Initialize()
{
	log::LoggingContext::GetInstance().AddLogListener(this);
	DisableLogLevels(log::ELevel::Verbose);
}

void ost::gui::OnscreenLogViewer::Uninitialize()
{
	log::LoggingContext::GetInstance().RemoveLogListener(this);
}

void ost::gui::OnscreenLogViewer::Display()
{
	int displayW = ImGui::GetIO().DisplaySize.x;
	int displayH = ImGui::GetIO().DisplaySize.y;

	int winW = displayW;
	int winH = displayH;
	ImGui::SetNextWindowSize({ (float)winW, (float)winH });
	ImGui::SetNextWindowPos({0.f, 0.f});

	ImGui::Begin("Log Viewer", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs );

	for (const log::LogMessage& msg : _displayMessages)
	{
		std::string loggerString = "";
		if (msg.LoggingInstancePtr != nullptr)
		{
			loggerString = log::FormatString("({})", msg.LoggingInstancePtr->GetInstanceName());
		}

		ImGui::TextColored(::s_lowFocusColor, msg.GetTimeStampString().c_str());
		ImGui::SameLine();
		ImGui::TextColored(::LogVerbosityToImColor(msg.MsgLevel), msg.Message.c_str());
	}

	ImGui::End();
}

void ost::gui::OnscreenLogViewer::NotifyMessage(const log::LogMessage& msg)
{
	_displayMessages.push_back(msg);

	if (_displayMessages.size() > _messageCount)
	{
		_displayMessages.erase(_displayMessages.begin()); // Remove the oldest entry
	}
}
