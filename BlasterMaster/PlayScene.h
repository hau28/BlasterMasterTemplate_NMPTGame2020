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

#include "PortalLib.h"


class CPlayScene: public CScene
{
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

	//SANH-CAMERA
	int NextSectionId;
	LPPORTAL toPortal;
	float offset_x_SectionSwitch;
	float offset_y_SectionSwitch;
	void set_offset(LPPORTAL fromPortal, LPPORTAL toPortal, string direction);

	//Sanh
	LPSECTION GetCurrentSection() { return Sections[CurrentSectionId]; }
	LPGAMEOBJECT GetPlayer() { return player; } 
	void SetPlayer(LPGAMEOBJECT newPlayer) { player = newPlayer; }
	void handleGameEvent(LPGAME_EVENT gameEvent);
	
	//friend class CPlayScenceKeyHandler;
};

typedef CPlayScene* LPPLAYSCENE;

