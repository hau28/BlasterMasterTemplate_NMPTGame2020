#pragma once
#include "Scene.h";
#include <iostream>
#include <fstream>
#include "Animations.h"

class CIntroScene : public CScene
{
protected:
	//Sanh code Input Sence
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);

public:
	CObjectAnimationHanlders animationHandlers;
	CIntroScene(int id, LPCWSTR filePath, int startupSection = -1);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
};
