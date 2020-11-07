#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include <string>

using namespace std;

class CScene
{
protected:
	CKeyEventHandler * key_handler;
	int id;
	LPCWSTR sceneFilePath;

public: 
	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;

	//Sanh code Input Sence
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATION(string line);
	void _ParseSection_COLLISION_BOXES(string line);
	void _ParseSection_SECTIONS(string line);
	void _ParseSection_CLASSES(string line);
	void _ParseSection_OBJECTS(string line);
};
typedef CScene * LPSCENE;

class CScenceKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void OnKeyHold() = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene *s) :CKeyEventHandler() { scence = s; }
};