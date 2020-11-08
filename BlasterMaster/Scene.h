#pragma once

#include <d3dx9.h>
#include <string>
#include "GameObject.h"
#include "Section.h"
#include "Consts.h"

using namespace std;

class CScene
{
protected:
	int id;
	LPCWSTR sceneFilePath;

public: 
	CScene() {};
	CScene(int id, LPCWSTR filePath, int startUpSection = -1);

	virtual void Load() = 0;
	virtual void Unload() = 0;
	void Update(DWORD dt);
	void Render();

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
	
	unordered_map<int, LPSECTION> Sections;
	int CurrentSectionId;
};
typedef CScene * LPSCENE;
