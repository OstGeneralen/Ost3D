#include "SignalTimer.h"

ost::SignalTimer::SignalTimer(float signalFreqSeconds, bool startRunning)
	: _nextSignalPoint{}
	, _signalFreqDuration{}
	, _signalFunction{}
	, _signalFrequencySeconds{signalFreqSeconds}
	, _isRunning(startRunning)
{
	auto secondsDur = std::chrono::duration<float>(signalFreqSeconds);
	_signalFreqDuration = std::chrono::duration_cast<std::chrono::milliseconds>(secondsDur);

	if (startRunning)
	{
		Start();
	}
}

void ost::SignalTimer::SetSignalFunction(std::function<void()> f)
{
	_signalFunction = f;
}

void ost::SignalTimer::Start()
{
	auto nextTimePoint = std::chrono::high_resolution_clock::now();
	nextTimePoint += _signalFreqDuration;
	
	_nextSignalPoint = nextTimePoint;
	_isRunning = true;
}

void ost::SignalTimer::Stop()
{
	_isRunning = false;
}

void ost::SignalTimer::Tick()
{
	if (_isRunning == false) return;
	
	if (std::chrono::high_resolution_clock::now() >= _nextSignalPoint)
	{
		if (_signalFunction)
		{
			_signalFunction();
		}
	
		Start();
	}
}