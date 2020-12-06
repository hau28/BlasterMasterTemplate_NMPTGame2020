#pragma once
#include "GameEvent.h"
class CJasonJumpInEvent : public CGameEvent
{
public:
	CJasonJumpInEvent() {};
	CJasonJumpInEvent(float x, float y, int sectionId)
	{
		this->eventName = "JasonJumpInEvent";
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