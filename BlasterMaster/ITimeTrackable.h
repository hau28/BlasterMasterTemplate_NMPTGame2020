#pragma once

class CTimer;

/// <summary>
/// An interface for every object that need to use the class Timer
/// </summary>
class ITimeTrackable
{
public:
	virtual void HandleTimerTick(CTimer* sender) = 0;
};

