#include "Timer.h"

CTimer::CTimer(ITimeTrackable* tracker, int interval, bool enableRepeat)
{
	this->tracker = tracker;
	this->interval = interval;
	this->enableRepeat = enableRepeat;
}

void CTimer::Update(int dt)
{
	if (!isRunning)
		return;

	if (timeElapsed < interval)
		timeElapsed += dt;

	if (timeElapsed >= interval)
	{
		// only handle tick event if last frame has not been ticked
		if (!lastFrameTicked)
		{
			tracker->HandleTimerTick(this);
			lastFrameTicked = true;
		}

		if (enableRepeat)
			Reset();
	}
}

void CTimer::Start()
{
	Reset();
	isRunning = true;
}

void CTimer::Continue()
{
	isRunning = true;
}

void CTimer::Reset()
{
	lastFrameTicked = false;
	timeElapsed = 0;
}

void CTimer::Stop()
{
	isRunning = false;
}
