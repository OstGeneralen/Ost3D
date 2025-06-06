#pragma once
#include <Engine/Utility/Time/SignalTimer.h>

namespace ost
{
	class FPSTracker
	{
	public:
		FPSTracker(size_t sampleSize, float sampleUpdateFrequency);
		
		void SetTolerance(float warnBelow);

		void TickTracker();

		void AddSample(float frameDuration);
		
		float GetCurrentFramesPerSeconds() const;
		float GetCurrentFrameTimeAverage() const;
	private:
		void Resample();

		SignalTimer _resampleTimer;
		float* _samples;
		size_t _numSamples;
		size_t _writeSample;

		float _calculatedFPS;
		float _frameTimeAverage;
		float _fpsTolerance;

	};
}