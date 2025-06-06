#include "Timer.h"
#include "SignalTimer.h"

ost::Timer::Timer()
	: _running(false)
{
}

void ost::Timer::Start()
{
	_startPoint = std::chrono::high_resolution_clock::now();
	_running = true;
}

void ost::Timer::Stop()
{
	_endPoint = std::chrono::high_resolution_clock::now();
	_running = false;
}

bool ost::Timer::IsRunning() const
{
	return _running;
}

float ost::Timer::GetDurationSeconds() const
{
	auto duration = _endPoint - _startPoint;
	auto secondsDuration = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	auto secondsDurF = std::chrono::duration_cast<std::chrono::duration<float>>(secondsDuration);
	return secondsDurF.count();
}
