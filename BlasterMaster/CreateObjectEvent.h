#pragma once
#include "GameEvent.h"
#include "GameObject.h"

class CCreateObjectEvent : public CGameEvent
{
public:
	CCreateObjectEvent() {};
	CCreateObjectEvent(LPGAMEOBJECT obj)
	{
		this->eventName = "CreateObjectEvent";
		this->gameObject = obj;
	}

	LPGAMEOBJECT gameObject = nullptr;
};

