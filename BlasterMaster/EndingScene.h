#pragma once
#include "Scene.h"
#include "Animations.h"

enum class StateEnding
{
	PEACE,
	EARTHQUAKE,
	FILM,
	CREDIT,
};

class CEndingScene : public CScene
{
	//Sanh code Input Sence
	StateEnding state;
	int ID_STATE; 
	int flag_EarthQuake = 0;
	float MountainX = 0, MountainY = 0;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);

public:
	void setState(StateEnding state);
	CObjectAnimationHanlders Film;
	CObjectAnimationHanlders Mountain;
	void HandleKeyEnter();
	CEndingScene(int id, LPCWSTR filePath, int startupSection = -1);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
};

