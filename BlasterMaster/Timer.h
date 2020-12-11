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
	ITimeTrackable* tracker;
	bool lastFrameTicked = false;

public:
	CTimer() {};
	CTimer(ITimeTrackable* tracker, int interval, bool enableRepeat);

	int interval = 0;
	bool enableRepeat = false;

	void Update(int dt);
	void Start();
	void Continue();
	void Reset();
	void Stop();
};

typedef CTimer* LPTIMER;

