#include "FPSTracker.h"

ost::FPSTracker::FPSTracker(size_t sampleSize, float sampleUpdateFreq)
	: _resampleTimer( sampleUpdateFreq )
	, _samples{new float[sampleSize]}
	, _numSamples{sampleSize}
	, _writeSample{0}
	, _calculatedFPS{0}
{
	_resampleTimer.SetSignalFunction([&] {Resample(); });
	_resampleTimer.Start();
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

void ost::FPSTracker::Resample()
{
	float samplesTotal = 0.0f;
	for (size_t i = 0; i < _numSamples; ++i)
	{
		samplesTotal += _samples[i];
	}

	float samplesAverage = samplesTotal / static_cast<float>(_numSamples);

	_calculatedFPS = 1.0f / samplesAverage;
}
