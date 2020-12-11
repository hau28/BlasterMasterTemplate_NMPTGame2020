#include "Timer.h"

CTimer::CTimer(ITimeTrackable* tracker, int interval)
{
	this->tracker = tracker;
	this->interval = interval;
	this->enableRepeatForever = true;
	this->repeatTimes = 1;

	Reset();
}

CTimer::CTimer(ITimeTrackable* tracker, int interval, int repeatTimes)
{
	this->tracker = tracker;
	this->interval = interval;
	this->enableRepeatForever = false;
	this->repeatTimes = repeatTimes;

	Reset();
}

void CTimer::Update(int dt)
{
	if (!isRunning)
		return;

	if (!enableRepeatForever && repeatTimesLeft == 0)
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
			
			if(!enableRepeatForever)
				repeatTimesLeft--;

			ResetClock();
		}
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

void CTimer::ResetClock()
{
	lastFrameTicked = false;
	timeElapsed = 0;
}

void CTimer::Reset()
{
	repeatTimesLeft = repeatTimes;
	ResetClock();
}

void CTimer::Stop()
{
	isRunning = false;
}
