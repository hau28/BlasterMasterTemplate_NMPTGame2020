#pragma once
#include "GameEvent.h";

class SwitchSceneEvent : public CGameEvent
{
private:
	int id_scene;

public:
	SwitchSceneEvent() { id_scene = 1; };
	SwitchSceneEvent(int ID) { id_scene = ID; };
	int getIDNextScene() { return id_scene; }
};

