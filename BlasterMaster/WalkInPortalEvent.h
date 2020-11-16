#pragma once
#include "GameEvent.h"
#include "PortalLib.h"

class CWalkInPortalEvent : public CGameEvent
{
	LPPORTAL fromPortal;
	LPPORTAL toPortal;
};

