#pragma once

#include <map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include "Scene.h"
#include "CustomKeyEvent.h"
#include "GameState.h"
#include "Utils.h"
#include "ControlKeys.h"
#include "GameEvent.h"
#include <iostream>
#include <fstream>

#define DIRECTINPUT_VERSION 0x0800
#define ID_SCENE_INTRO 1
#define ID_SCENE_SIDEVIEW 2
#define ID_SCENE_OVERHEAD 3
#define ID_SCENE_END 4

#define PORTAL_TO_ENDSCENE_PORT 0

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGameObject;

/// <summary>
/// This class manage universal game logic, it just should be called GameManager
/// 
/// </summary>
class CGame
{
	static GameState state;
	static GameState flagState;

	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = nullptr;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = nullptr;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = nullptr;		
	LPD3DXSPRITE spriteHandler = nullptr;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	// LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height; 

	// std::map<int, LPSCENE> scenes;
	std::map<int, LPSCENE> scenes;
	int current_scene; 

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

	static vector<LPGAME_EVENT> gameEvents;

public:
	//Background Color
	static D3DCOLOR BackgroundColor;
	static void setBackGroundColor(int R, int G, int B)
	{
		BackgroundColor = D3DCOLOR_XRGB(R, G, B);
	}

	void InitKeyboard();
	// void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);

	/// <summary>
	/// Draw a sprite to game world
	/// </summary>
	/// <param name="x, y">Position to render, relative to the game world</param>
	/// <param name="texture">Source spritesheet</param>
	/// <param name="left, right, top, bottom">the range of sprite in the spritesheet</param>
	/// <param name="alpha">the transparency</param>
	/// <param name="flipX"></param>
	/// <param name="rotate">times to rotate 90 degre clockwise</param>
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255, bool flipX = false, int rotate = 0, float offset_x = 0, float offset_y = 0, int modifyR = 255, int modifyG = 255, int modifyB = 255);
	 
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	vector<CCustomKeyEvent*> CustomKeyEvents; // Just like keyEvents, but friendlier to use :)

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; };
	int GetCurrentSceneId() { return current_scene; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	/// <summary>
	/// <para> Returns the current player of the current scene </para>
	/// <para> Returns null if there's no player in the current scene </para>
	/// <para> Becareful: not check the game state yet </para>
	/// </summary>
	CGameObject* GetCurrentPlayer();

	void SetCamPos(float x, float y) { cam_x =  x; cam_y = y; }
	void GetCamPos(float &x, float &y) { x = cam_x; y = cam_y; }

	static GameState GetState() { return state; };
	static void SetState(GameState newState);

	static GameState GetFlagState() { return flagState; };
	static void SetFlagState(GameState newState) { flagState = newState; };

	static void AddGameEvent(LPGAME_EVENT gameEvent);
	static void HandleGameEvents();
	static void HandleGameEvent(LPGAME_EVENT gameEvent);

	static CGame * GetInstance();
	~CGame();
};


