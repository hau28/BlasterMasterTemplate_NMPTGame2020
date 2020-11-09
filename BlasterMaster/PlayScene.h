#pragma once

#include <iostream>
#include <fstream>

#include "Game.h"
#include "Scene.h"
#include "Textures.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Section.h"

#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"


class CPlayScene: public CScene
{

#pragma region Scene
protected: 
	LPGAMEOBJECT player;					// A play scene has to have player, right? 


	//void _ParseSection_TEXTURES(string line);
	//void _ParseSection_SPRITES(string line);
	//void _ParseSection_ANIMATIONS(string line);
	//void _ParseSection_ANIMATION_SETS(string line);
	//void _ParseSection_OBJECTS(string line);

	//Sanh code Input Sence
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);
	void _ParseSection_COLLISION_BOXES(string line);
	void _ParseSection_SECTIONS(string line);
	void _ParseSection_CLASSES(string line);
	void _ParseSection_OBJECTS(string line);
	
public: 
	CPlayScene(int id, LPCWSTR filePath, int startupSection = -1);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	unordered_map<int, LPSECTION> Sections;
	int CurrentSectionId;

	//Sanh
	LPSECTION GetCurrentSection() { return Sections[CurrentSectionId]; }

	LPGAMEOBJECT GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};
#pragma endregion


