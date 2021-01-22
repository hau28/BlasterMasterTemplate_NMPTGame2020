#pragma once

#pragma once
#include "GameEvent.h"

class CComeBackAfterCrusherEvent : public CGameEvent
{
	public:
	int oldSection;
	CComeBackAfterCrusherEvent() {}
	CComeBackAfterCrusherEvent(int oldSection)
	{
		this->oldSection = oldSection;
	}
};

