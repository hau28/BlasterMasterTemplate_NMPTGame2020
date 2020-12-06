#pragma once
#include "GameEvent.h"
#include "PortalLib.h"

class CWalkInPortalEvent : public CGameEvent
{
	LPPORTAL fromPortal;
	LPPORTAL toPortal;

public:
	CWalkInPortalEvent() {}
	//CWalkInPortalEvent(const CWalkInPortalEvent& gameEvent) 
	//{
	//	eventName = gameEvent.eventName;
	//	fromPortal = gameEvent.fromPortal;
	//	toPortal = gameEvent.toPortal;
	//}

	CWalkInPortalEvent(string event, LPPORTAL portal1, LPPORTAL portal2)
	{
		this->eventName = event;
		this->fromPortal = portal1;
		this->toPortal = portal2;
	}
	LPPORTAL get_toPortal() { return toPortal; };
	LPPORTAL get_fromPortal() { return fromPortal; };
};

