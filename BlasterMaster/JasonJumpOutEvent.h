#pragma once
#include "GameEvent.h"

class CJasonJumpOutEvent : public CGameEvent
{
public:
	CJasonJumpOutEvent() {};
	CJasonJumpOutEvent(float xCenter, float yCenter, int sectionId)
	{
		this->eventName = "JasonJumpOutEvent";
		this->xCenter = xCenter;
		this->yCenter = yCenter;
		this->sectionId = sectionId;
	}

	/// <summary>
	/// Jason initial position
	/// </summary>
	float xCenter, yCenter;

	/// <summary>
	/// Jason initial section Id
	/// </summary>
	int sectionId;

	bool turnRight;
};

