#pragma once
#include "ITimeTrackable.h"


/// <summary>
/// Quick guide to use timer: init, update, handletimertick
/// </summary>
class CTimer
{
private:
	int timeElapsed = 0;
	int isRunning = true;
	ITimeTrackable* tracker = nullptr;
	bool lastFrameTicked = false;
	int repeatTimes = 1;

	void ResetClock();

public:
	CTimer() {};
	/// <summary>
	/// Initialize a timer, which repeats forever
	/// </summary>
	/// <param name="tracker"></param>
	/// <param name="interval"></param>
	CTimer(ITimeTrackable* tracker, int interval);

	/// <summary>
	/// Initialize a timer, which repeats a given amount of time
	/// </summary>
	/// <param name="tracker"></param>
	/// <param name="interval"></param>
	/// <param name="repeatTimes"></param>
	CTimer(ITimeTrackable* tracker, int interval, int repeatTimes);

	// dangerous to set these value... -> Violate encapsulation
	int interval = 0;
	bool enableRepeatForever = false;
	int repeatTimesLeft = 1;

	void Update(int dt);
	void Start();
	void Continue();
	void Reset();
	void Stop();

	~CTimer() {}
};

typedef CTimer* LPTIMER;

