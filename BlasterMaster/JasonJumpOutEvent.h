#pragma once
#include "GameEvent.h"

class CJasonJumpOutEvent : public CGameEvent
{
public:
	CJasonJumpOutEvent() {};
	CJasonJumpOutEvent(float x, float y, int sectionId)
	{
		this->eventName = "JasonJumpOutEvent";
		this->x = x;
		this->y = y;
		this->sectionId = sectionId;
	}

	/// <summary>
	/// Jason initial position
	/// </summary>
	float x, y;

	/// <summary>
	/// Jason initial section Id
	/// </summary>
	int sectionId;

	void virtualfunc() {};
};

