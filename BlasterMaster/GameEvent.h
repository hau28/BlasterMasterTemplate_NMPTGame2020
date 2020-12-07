#pragma once
#include <string>

class CGameEvent
{
	public:
	std::string eventName;
	CGameEvent() {};
	CGameEvent(std::string event) { this->eventName = event; }

	virtual ~CGameEvent() {};
};

typedef CGameEvent* LPGAME_EVENT;
