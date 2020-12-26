#pragma once
#include "GameEvent.h";
#include "Portal.h"

class SwitchSceneEvent : public CGameEvent
{
private:
	int id_scene;
	LPPORTAL portal = nullptr;

public:
	SwitchSceneEvent() { id_scene = 1; };
	SwitchSceneEvent(LPPORTAL portal);
	SwitchSceneEvent(int ID) { id_scene = ID; };
	int getIDNextScene() { return id_scene; }
};

