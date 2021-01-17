#pragma once
#include "dsound.h"
#include "windows.h"
#include <map>
#include <string>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//DECLARE SOUND FILE NAME
const std::string AREA2 = "area2";
const std::string SMALL_PINK_BULLET_EXPLOSION = "small_pink_bullet_explosion";
const std::string ENEMY_DIE = "enemy_die";
const std::string JASON_GOT_HIT = "jason_got_hit";
const std::string JASON_DIE = "jason_die";
const std::string PANDA_MOVING = "panda_moving";
const std::string PANDA_JUMP_SOUND = "panda_jump";
const std::string DOME_FLY = "dome_fly";
const std::string MENU = "menu";
const std::string SOPHIA_GOT_HIT = "sophia_got_hit";
const std::string SOPHIA_DIE = "sophia_die";
const std::string SOPHIA_SHOOT = "sophia_shoot";
const std::string SOPHIA_JUMP = "sophia_jump";
const std::string SOPHIA_LAND = "sophia_land"; 
const std::string MENU_SELECT = "menu_select";
const std::string ENEMY_GOT_HIT = "enemy_got_hit";
const std::string SOPHIA_BULLET_EXPLOSION = "sophia_bullet_explosion";
const std::string SWAP_PLAYER = "swap_player";
const std::string JASON_JUMP = "jason_jump";
const std::string JASON_SIDEVIEW_SHOOT = "jason_sideview_shoot";
const std::string BUZZ = "buzz";
const std::string SKULL_BOMB = "skull_bomb";
const std::string MINE = "mine";
const std::string POWERUP = "powerup";
const std::string SWITCH_SCENE = "switch_scene";
const std::string ENEMY_OVERHEAD_SHOOT = "enemy_overhead_shoot";
const std::string TELEPORTER_SWITCH = "teleporter_switch";


class Sound
{
public:
	struct WaveHeaderStruct
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
	float volume;
	void static create(HWND hWnd);
	void setVolume(float percentage, std::string name = "");
	void loadSound(std::string name);
	void loadGameSounds();
	void play(std::string name, bool infiniteLoop, int times);
	void stop(std::string name = "");
	float getVolume();
	~Sound();
	static Sound* getInstance();
	void mute();
	void unMute();
	void cleanUp();
private:
	Sound(HWND hWnd);
	static Sound* instance;
	IDirectSound8* pDevice;
	IDirectSoundBuffer* primaryBuffer;
	std::map<std::string, IDirectSoundBuffer8*> soundBufferMap;
	bool isMute;
};

