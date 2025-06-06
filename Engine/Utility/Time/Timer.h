#pragma once
#include <chrono>

namespace ost
{
	class Timer
	{
	public:
		Timer();

		void Start();
		void Stop();

		bool IsRunning() const;

		float GetDurationSeconds() const;
	private:
		std::chrono::high_resolution_clock::time_point _startPoint;
		std::chrono::high_resolution_clock::time_point _endPoint;
		bool _running;
	};
}