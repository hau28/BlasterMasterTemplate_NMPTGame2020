#pragma once
#include "GameEvent.h"
#include "Section.h"

class CSection;
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
	/*void deleteJasonSideview();
	unordered_map<int, LPSECTION> Sections;*/
};