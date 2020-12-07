#pragma once
#include "GameEvent.h"
#include "GameObject.h"

class CRemoveObjectEvent : public CGameEvent
{
public:
	CRemoveObjectEvent() {};
	CRemoveObjectEvent(LPGAMEOBJECT obj, bool toBeDeleted = true)
	{
		this->eventName = "RemoveObjectEvent";
		this->gameObject = obj;
		this->toBeDeleted = toBeDeleted;
	}

	LPGAMEOBJECT gameObject = nullptr;
	bool toBeDeleted;
};

