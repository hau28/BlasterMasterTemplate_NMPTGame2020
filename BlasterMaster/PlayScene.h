#pragma once

#include <iostream>
#include <fstream>

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"

#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"


class CPlayScene: public CScene
{

#pragma region Scene
protected: 
	// CMario *player;					// A play scene has to have player, right? 


	//void _ParseSection_TEXTURES(string line);
	//void _ParseSection_SPRITES(string line);
	//void _ParseSection_ANIMATIONS(string line);
	//void _ParseSection_ANIMATION_SETS(string line);
	//void _ParseSection_OBJECTS(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	// CMario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};
#pragma endregion


