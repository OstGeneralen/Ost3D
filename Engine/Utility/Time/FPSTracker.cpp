#include "FPSTracker.h"
#include <Engine/Utility/Logging/Logging.h>

STATIC_LOG(FpsTrackLog);

ost::FPSTracker::FPSTracker(size_t sampleSize, float sampleUpdateFreq)
	: _resampleTimer( sampleUpdateFreq )
	, _samples{new float[sampleSize]}
	, _numSamples{sampleSize}
	, _writeSample{0}
	, _calculatedFPS{0}
	, _fpsTolerance{30}
{
	_resampleTimer.SetSignalFunction([&] {Resample(); });
	_resampleTimer.Start();
}

void ost::FPSTracker::SetTolerance(float warnBelow)
{
	_fpsTolerance = warnBelow;
}

void ost::FPSTracker::TickTracker()
{
	_resampleTimer.Tick();
}

void ost::FPSTracker::AddSample(float frameDuration)
{
	_samples[_writeSample] = frameDuration;
	_writeSample = (_writeSample + 1) % _numSamples;
}

float ost::FPSTracker::GetCurrentFramesPerSeconds() const
{
	return _calculatedFPS;
}

float ost::FPSTracker::GetCurrentFrameTimeAverage() const
{
	return _frameTimeAverage;
}

void ost::FPSTracker::Resample()
{
	float samplesTotal = 0.0f;
	for (size_t i = 0; i < _numSamples; ++i)
	{
		samplesTotal += _samples[i];
	}

	_frameTimeAverage = samplesTotal / static_cast<float>(_numSamples);

	_calculatedFPS = 1.0f / _frameTimeAverage;

	if (_calculatedFPS <= _fpsTolerance)
	{
		FpsTrackLog.LOG_WARNING("Framerate below the set tolerance detected (Recorded FPS: {}, Tolerance: {})", _calculatedFPS, _fpsTolerance);
	}
}
