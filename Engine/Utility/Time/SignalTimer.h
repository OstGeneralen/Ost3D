#pragma once
#include <chrono>
#include <functional>

namespace ost
{
	class SignalTimer
	{
	public:
		SignalTimer(float signalFreqSeconds, bool startRunning = false);

		void SetSignalFunction(std::function<void()> f);

		void Start();
		void Stop();
		void Tick();
	private:
		std::chrono::high_resolution_clock::time_point _nextSignalPoint;
		std::chrono::milliseconds _signalFreqDuration;
		std::function<void()> _signalFunction;
		float _signalFrequencySeconds;
		bool _isRunning;
	};
}