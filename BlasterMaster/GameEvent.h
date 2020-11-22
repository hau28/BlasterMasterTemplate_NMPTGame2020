#pragma once
#include <string>

class CGameEvent
{
	public:
	std::string eventName;
	CGameEvent() {};
	CGameEvent(std::string event) { this->eventName = event; }
	virtual void virtualfunc() = 0;
};

typedef CGameEvent* LPGAME_EVENT;
